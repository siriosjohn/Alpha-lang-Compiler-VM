#include "symtable.h"

#define INIT_TABLE_SIZE 256
#define HASH_MULT 15711

int exit_flag = 0;

scoped_entries_list *scope_list_head = NULL;

unsigned int variable_hash(Variable *entry, int buckets) {
    size_t ui;
    unsigned int uiHash = 1337;
    for (ui = 0; entry->name[ui] != '\0'; ui++)
        uiHash = uiHash * HASH_MULT + entry->name[ui] * entry->line;
    return uiHash % buckets;
}

unsigned int function_hash(Function *entry, int buckets) {
    size_t ui;
    unsigned int uiHash = 1337;
    for (ui = 0; entry->name[ui] != '\0'; ui++)
        uiHash = uiHash * HASH_MULT + entry->name[ui] * entry->line;
    return uiHash % buckets;
}

SymTable *create_table() {
    int i;
    SymTable *table = (SymTable *)malloc(sizeof(SymTable));
    table->HashTable = (SymbolTableEntry **)malloc(INIT_TABLE_SIZE * sizeof(SymbolTableEntry));
    for(i = 0; i < INIT_TABLE_SIZE; i++)
        table->HashTable[i] = NULL;
    table->curr_size = 0;
    table->buckets = INIT_TABLE_SIZE;

    function_insert(table, create_function("print", 0, 0), LIBFUNC);
    function_insert(table, create_function("input", 0, 0), LIBFUNC);
    function_insert(table, create_function("objectmemberkeys", 0, 0), LIBFUNC);
    function_insert(table, create_function("objecttotalmembers", 0, 0), LIBFUNC);
    function_insert(table, create_function("objectcopy", 0, 0), LIBFUNC);
    function_insert(table, create_function("totalarguments", 0, 0), LIBFUNC);
    function_insert(table, create_function("argument", 0, 0), LIBFUNC);
    function_insert(table, create_function("typeof", 0, 0), LIBFUNC);
    function_insert(table, create_function("strtonum", 0, 0), LIBFUNC);
    function_insert(table, create_function("sqrt", 0, 0), LIBFUNC);
    function_insert(table, create_function("cos", 0, 0), LIBFUNC);
    function_insert(table, create_function("sin", 0, 0), LIBFUNC);

    return table;
}

SymbolTableEntry *variable_insert(SymTable *table, Variable *var, enum SymbolType type) {
    SymbolTableEntry *tmp = NULL;
    SymbolTableEntry *new = (SymbolTableEntry *)malloc(sizeof(SymbolTableEntry));
    unsigned int spot = -1;
    spot = variable_hash(var, table->buckets);

    if (!new || spot == -1)
        return NULL;

    new->isVariable = true;
    new->isActive = true;
    new->value.varVal = var;
    new->type = type;

    new->scope_next = NULL;
    new->scope_prev = NULL;
    new->table_next = NULL;
    

    if (!table->HashTable[spot]) {
        table->HashTable[spot] = new;
        table->curr_size++;
    } else {
        tmp = table->HashTable[spot];
        while (tmp->table_next != NULL) {
            tmp = tmp->table_next;
        }
        tmp->table_next = new;
        table->curr_size++;
    }

    /*keep track of scope list*/
    if (!scope_list_head) {
        scope_list_head = (scoped_entries_list *)malloc(sizeof(scoped_entries_list));
        scope_list_head->scope_head = new;
        scope_list_head->scope_tail = new;
        scope_list_head->scope = var->scope;
    } else {
        scoped_entries_list *tmp = scope_list_head;
        while (tmp->next != NULL) {
            if (tmp->scope == var->scope) {
                break;
            }
            tmp = tmp->next;
        }
        if (tmp->scope == var->scope) {
            tmp->scope_tail->scope_next = new;
            new->scope_prev = tmp->scope_tail;
            tmp->scope_tail = new;
        } else if (tmp->next == NULL) {
            scoped_entries_list *new_obj = (scoped_entries_list *)malloc(sizeof(scoped_entries_list));
            tmp->next = new_obj;
            new_obj->next = NULL;
            new_obj->scope_head = new;
            new_obj->scope_tail = new;
            new_obj->scope = var->scope;
        }
    }

    inccurrscopeoffset();    

    return new;
}

SymbolTableEntry *function_insert(SymTable *table, Function *func, enum SymbolType type) {
    SymbolTableEntry *tmp_entry = NULL;
    SymbolTableEntry *new = (SymbolTableEntry *)malloc(sizeof(SymbolTableEntry));
    unsigned int spot = -1;
    spot = function_hash(func, table->buckets);

    if (new == NULL || spot == -1)
        return NULL;

    new->isVariable = false;
    new->isActive = true;
    new->value.funcVal = func;
    new->type = type;

    new->scope_next = NULL;
    new->scope_prev = NULL;
    new->table_next = NULL;

    if (table->HashTable[spot] == NULL) {
        table->HashTable[spot] = new;
        table->curr_size++;
    } else {
        tmp_entry = table->HashTable[spot];
        while (tmp_entry->table_next != NULL) {
            tmp_entry = tmp_entry->table_next;
        }
        tmp_entry->table_next = new;
        table->curr_size++;
    }

    /*keep track of scope list*/
    if (scope_list_head == NULL) {
        scope_list_head = (scoped_entries_list *)malloc(sizeof(scoped_entries_list));
        scope_list_head->scope_head = new;
        scope_list_head->scope_tail = new;
        scope_list_head->scope = func->scope;
        scope_list_head->next = NULL;
    } else {
        scoped_entries_list *tmp = scope_list_head;
        while (tmp->next != NULL) {
            if (tmp->scope == func->scope) {
                break;
            }
            tmp = tmp->next;
        }
        if (tmp->scope == func->scope) {
            tmp->scope_tail->scope_next = new;
            new->scope_prev = tmp->scope_tail;
            tmp->scope_tail = new;
        } else if (tmp->next == NULL) {
            scoped_entries_list *new_obj = (scoped_entries_list *)malloc(sizeof(scoped_entries_list));
            tmp->next = new_obj;
            new_obj->scope_head = new;
            new_obj->scope_tail = new;
            new_obj->scope = func->scope;
        }
    }

    return new;
}

enum SymbolType distinguish_lookup(SymTable *table, const char *name, unsigned int curr_scope) {
    scoped_entries_list *tmp = scope_list_head;
    SymbolTableEntry *tmp_entry = tmp->scope_tail;
    int tmp_scope = curr_scope;

    // fprintf(stderr, "Distinguish called for %s\n", name);

    while (tmp_scope >= 0) {
        tmp = scope_list_head;
        while (tmp != NULL) {
            if (tmp->scope != tmp_scope) {
                tmp = tmp->next;
                continue;
            } else {
                // fprintf(stderr, "scope: %d\n", tmp_scope);
                tmp_entry = tmp->scope_tail;
                while (tmp_entry != NULL) {
                    // fprintf(stderr, "looking at: %s\n", tmp_entry->value.varVal->name);
                    if (tmp_entry->isActive == false && tmp_entry->isVariable == true) {
                        tmp_entry = tmp_entry->scope_prev;
                        continue;
                    }
                    if (strcmp(name, tmp_entry->value.varVal->name) == 0) {
                        if (tmp_entry->type == USERFUNC || tmp_entry->type == LIBFUNC) {
                            // fprintf(stderr, "matched with \"%s\" -- <%d>\n", tmp_entry->value.varVal->name, tmp_entry->value.varVal->line);
                            return tmp_entry->type;
                        }
                        // fprintf(stderr, "matched with \"%s\" -- <%d>\n", tmp_entry->value.varVal->name, tmp_entry->value.varVal->line);
                        return GENERAL;
                    }
                    tmp_entry = tmp_entry->scope_prev;
                }
            }
            tmp = tmp->next;
        }
        tmp_scope--;
    }

    return GENERAL;
}

/* Lookup for a Variable */
SymbolTableEntry *variable_lookup(SymTable *table, Variable *var, enum SymbolType type, unsigned int curr_scope) {
    scoped_entries_list *tmp = scope_list_head;
    SymbolTableEntry *tmp_entry = tmp->scope_head;
    int tmp_scope = curr_scope;

    /*lookup for temp variables*/
    if (type == TEMP_VAR) {
        while (tmp != NULL) {
            if (tmp->scope != curr_scope) {
                tmp = tmp->next;
                continue;
            } else {
                tmp_entry = tmp->scope_head;
                while (tmp_entry != NULL) {
                    if (tmp_entry->isActive != true) {
                        tmp_entry = tmp_entry->scope_next;
                        continue;
                    }
                    if (strcmp(var->name, tmp_entry->value.varVal->name) == 0) {
                        return tmp_entry;
                    }
                    tmp_entry = tmp_entry->scope_next;
                }
            }
            tmp = tmp->next;
        }
        SymbolTableEntry *new = variable_insert(table, var, LOCAL_t);
        return new;
    }

    /*lookup for local variables*/
    if (type == LOCAL_t) {
        while (tmp != NULL) {
            if (tmp->scope != curr_scope) {
                tmp = tmp->next;
                continue;
            } else {
                tmp_entry = tmp->scope_head;
                while (tmp_entry != NULL) {
                    if (tmp_entry->isActive == false) {
                        tmp_entry = tmp_entry->scope_next;
                        continue;
                    }
                    if (strcmp(var->name, tmp_entry->value.varVal->name) == 0) {
                        if (nameAllowed(var->name) == false) {
                            fprintf(stderr, "Error, \"%s\", line <%d>, shadows library function.\n", var->name, var->line);
                            exit_flag = 1;
                            return tmp_entry;
                        }
                        return tmp_entry;
                    }
                    tmp_entry = tmp_entry->scope_next;
                }
            }
            tmp = tmp->next;
        }

        if (nameAllowed(var->name) == false) {
            fprintf(stderr, "Error, \"%s\", line <%d>, shadows library function.\n", var->name, var->line);
            exit_flag = 1;
            return NULL;
        } else {
            if (var->scope == 0) {
                SymbolTableEntry *new = variable_insert(table, var, GLOBAL);
            } else {
                SymbolTableEntry *new = variable_insert(table, var, type);
            }
        }
    }

    /*lookup for global variables*/
    if (type == GLOBAL) {
        while (tmp != NULL) {
            if (tmp->scope != 0) {
                tmp = tmp->next;
                continue;
            } else {
                tmp_entry = tmp->scope_head;
                while (tmp_entry != NULL) {
                    if (tmp_entry->isActive == false) {
                        tmp_entry = tmp_entry->scope_next;
                        continue;
                    }
                    if (strcmp(var->name, tmp_entry->value.varVal->name) == 0) {
                        return tmp_entry;
                    }
                    tmp_entry = tmp_entry->scope_next;
                }
            }
            tmp = tmp->next;
        }

        fprintf(stderr, "Error, \"%s\" at line <%d> undeclared, first used here.\n", var->name, var->line);
        exit_flag = 1;
        return NULL;
    }

    /*general variable lookup*/
    if (type == GENERAL) {
        // fprintf(stderr, "Lookup called for %s\n", var->name);
        while (tmp_scope >= 0) {
            // fprintf(stderr, "scope: %d\n", tmp_scope);
            tmp = scope_list_head;
            while (tmp != NULL) {
                if (tmp->scope != tmp_scope) {
                    tmp = tmp->next;
                    continue;
                } else {
                    tmp_entry = tmp->scope_tail;
                    while (tmp_entry != NULL) {
                        // fprintf(stderr, "looking at: %s\n", tmp_entry->value.varVal->name);
                        if (tmp_entry->isActive == false) {
                            tmp_entry = tmp_entry->scope_prev;
                            continue;
                        }
                        if (strcmp(var->name, tmp_entry->value.varVal->name) == 0) {
                            if (tmp_entry->type != GLOBAL) {
                                if (active_function_between_scopes(tmp_entry->value.varVal->scope, var->scope) == 1) {
                                    fprintf(stderr, "Error, cannot access \"%s\" previous decleration was here <%d>\n", var->name, tmp_entry->value.varVal->line);
                                    exit_flag = 1;
                                    return tmp_entry;
                                }
                            }

                            // fprintf(stderr, "matched with \"%s\" -- <%d>\n", tmp_entry->value.varVal->name, tmp_entry->value.varVal->line);
                            return tmp_entry;
                        }
                        tmp_entry = tmp_entry->scope_prev;
                    }
                }
                tmp = tmp->next;
            }
            tmp_scope--;
        }

        /*nothing found, insert var in symbol table*/
        // fprintf(stderr, "Nothn found, inserting\n--------\n");
        if (var->scope == 0) {
            SymbolTableEntry *new = variable_insert(table, var, GLOBAL);
            return new;
        } else {
            SymbolTableEntry *new = variable_insert(table, var, type);
            return new;
        }
    }
}

/* Lookup for a Function */
SymbolTableEntry *function_lookup(SymTable *table, Function *func, enum SymbolType type, unsigned int curr_scope, bool isCall) {
    scoped_entries_list *tmp = scope_list_head;
    SymbolTableEntry *tmp_entry = tmp->scope_head;
    int tmp_scope = curr_scope;
    if (nameAllowed(func->name) == false) {
        if (isCall == false) {
            exit_flag = 1;
            fprintf(stderr, "Error, \"%s\" at line <%d> shadows library function\n", func->name, func->line);
        }
        return NULL;
    }
    if (isCall == true) {
        while (tmp_scope >= 0) {
            tmp = scope_list_head;
            while (tmp != NULL) {
                if (tmp->scope != tmp_scope) {
                    tmp = tmp->next;
                    continue;
                } else {
                    tmp_entry = tmp->scope_tail;
                    while (tmp_entry != NULL) {
                        // if (tmp_entry->isActive == false) {
                        //     tmp_entry = tmp_entry->scope_prev;
                        //     continue;
                        // }
                        if (strcmp(func->name, tmp_entry->value.funcVal->name) == 0) {
                            if (tmp_entry->type == USERFUNC || tmp_entry->type == LIBFUNC) {
                                return tmp_entry;
                            }
                        }
                        tmp_entry = tmp_entry->scope_prev;
                    }
                }
                tmp = tmp->next;
            }
            tmp_scope--;
        }
    }

    /* search the list to find the scope of the func that we want to lookup */
    while (tmp != NULL) {
        if (tmp->scope != curr_scope) {
            tmp = tmp->next;
            continue;
        } else {
            tmp_entry = tmp->scope_head;
            while (tmp_entry != NULL) {
                if (tmp_entry->isActive == false) {
                    tmp_entry = tmp_entry->scope_next;
                    continue;
                }
                if (strcmp(func->name, tmp_entry->value.funcVal->name) == 0) {
                    if (tmp_entry->type == USERFUNC){
                        fprintf(stderr, "Error, redefinition of \"%s\" at line <%d>, previous definition was at line <%d>\n", func->name, func->line, tmp_entry->value.funcVal->line);
                        exit_flag = 1;
                    }
                    else{
                        fprintf(stderr, "Error, \"%s\" redeclared as different kind of symbol, previous definition was here <%d>\n", func->name, tmp_entry->value.funcVal->line);
                        exit_flag = 1;
                    }
                    return tmp_entry;
                }
                tmp_entry = tmp_entry->scope_next;
            }
        }
        tmp = tmp->next;
    }
    SymbolTableEntry *new;
    if (isCall == false)
        new = function_insert(table, func, USERFUNC);

    return new;
}

SymbolTableEntry *formal_lookup(SymTable *table, Variable *var, enum SymbolType type, unsigned int curr_scope) {
    scoped_entries_list *tmp = scope_list_head;
    SymbolTableEntry *tmp_entry = tmp->scope_head;

    while (tmp != NULL) {
        if (tmp->scope != curr_scope) {
            tmp = tmp->next;
            continue;
        } else {
            tmp_entry = tmp->scope_head;
            while (tmp_entry != NULL) {
                if (tmp_entry->isActive == false) {
                    tmp_entry = tmp_entry->scope_next;
                    continue;
                }
                if (strcmp(var->name, tmp_entry->value.varVal->name) == 0) {
                    fprintf(stderr, "Error, redeclaration of formal argument \"%s\"\n", var->name);
                    exit_flag = 1;
                    return tmp_entry;
                }

                tmp_entry = tmp_entry->scope_next;
            }
        }
        tmp = tmp->next;
    }

    SymbolTableEntry *new;
    if (nameAllowed(var->name) == true) {
        new = variable_insert(table, var, type);
        return new;
    } else {
        fprintf(stderr, "Error, redefinition of library function \"%s\" at line <%d>\n", var->name, var->line);
        exit_flag = 1;
    }
}

SymbolTableEntry *libfunc_lookup(SymTable *table, const char *name) {
    scoped_entries_list *tmp = scope_list_head;
    SymbolTableEntry *tmp_entry = tmp->scope_head;

    while (tmp != NULL) {
        if (tmp->scope != 0) {
            tmp = tmp->next;
            continue;
        } else {
            tmp_entry = tmp->scope_head;
            while (tmp_entry != NULL) {
                if (strcmp(name, tmp_entry->value.varVal->name) == 0)
                    return tmp_entry;

                tmp_entry = tmp_entry->scope_next;
            }
        }
        tmp = tmp->next;
    }
}

int active_function_between_scopes(unsigned int scope_low, unsigned int scope_high) {
    scoped_entries_list *tmp = scope_list_head;
    SymbolTableEntry *tmp_entry = tmp->scope_head;

    while (tmp != NULL) {
        tmp_entry = tmp->scope_head;
        if (tmp->scope >= scope_low && tmp->scope <= scope_high) {
            while (tmp_entry != NULL) {
                if (tmp_entry->type == USERFUNC && tmp_entry->isActive == true) {
                    return 1;
                }
                tmp_entry = tmp_entry->scope_next;
            }
        }
        tmp = tmp->next;
    }

    return 0;
}

bool nameAllowed(const char *name) {
    if (strcmp(name, "print") == 0 || strcmp(name, "input") == 0 || strcmp(name, "objectmemberkeys") == 0 || strcmp(name, "objecttotalmembers") == 0 ||
        strcmp(name, "objectcopy") == 0 || strcmp(name, "totalarguments") == 0 || strcmp(name, "argument") == 0 || strcmp(name, "typeof") == 0 ||
        strcmp(name, "strtonum") == 0 || strcmp(name, "sqrt") == 0 || strcmp(name, "cos") == 0 || strcmp(name, "sin") == 0) {
        return false;
    }

    return true;
}

int hide(SymTable *table, unsigned int scope) {
    scoped_entries_list *tmp = scope_list_head;
    bool scope_found = false;
    while (tmp->next != NULL) {
        if (tmp->scope != scope) {
            tmp = tmp->next;
            continue;
        }
        scope_found = true;
        break;
    }

    if (tmp->scope == scope) {
        SymbolTableEntry *entry = tmp->scope_head;
        while (entry != NULL) {
            entry->isActive = false;
            entry = entry->scope_next;
        }
    }
    return scope_found;
}

int hide_tmp_var(SymTable *table, unsigned int scope) {
    scoped_entries_list *tmp = scope_list_head;
    bool scope_found = false;
    while (tmp->next != NULL) {
        if (tmp->scope != scope) {
            tmp = tmp->next;
            continue;
        }
        scope_found = true;
        break;
    }

    if (tmp->scope == scope) {
        SymbolTableEntry *entry = tmp->scope_head;
        while (entry != NULL) {
            if (entry->value.varVal->name[0] == '^') {
                entry->isActive = false;
            }

            entry = entry->scope_next;
        }
    }
    return scope_found;
}

int greatest_tmp_var(int scope) {
    int largest_number = -1;
    scoped_entries_list *tmp = scope_list_head;
    while (tmp->next != NULL) {
        if (tmp->scope != scope) {
            tmp = tmp->next;
            continue;
        }
        break;
    }
    SymbolTableEntry *entry = tmp->scope_head;
    while (entry != NULL) {
        if (entry->value.varVal->name[0] == '^') {
            if (entry->isActive == true) {
                printf("looking at: %s\n", entry->value.varVal->name);
                if (largest_number < entry->value.varVal->name[1] - 48) {
                    largest_number = entry->value.varVal->name[1] - 48;
                }
            }
        }
        entry = entry->scope_next;
    }
    return largest_number;
}

void print_table_scopes(SymTable *table, FILE *stream) {
    scoped_entries_list *head = scope_list_head;
    SymbolTableEntry *st_entry = head->scope_head;
    while (head != NULL) {
        fprintf(stream, "----------     scope #%d      ----------\n", head->scope);
        st_entry = head->scope_head;
        while (st_entry != NULL) {
            if (st_entry->isVariable)
                fprintf(stream, "\"%s\" [%s] (line %d) (scope %d) [%s] (offset %d)\n", st_entry->value.funcVal->name, toString(st_entry->type), st_entry->value.funcVal->line, st_entry->value.funcVal->scope, spacetoString(st_entry->value.varVal->space), st_entry->value.varVal->offset);
            else
                fprintf(stream, "\"%s\" [%s] (line %d) (scope %d)\n", st_entry->value.funcVal->name, toString(st_entry->type), st_entry->value.funcVal->line, st_entry->value.funcVal->scope);

            st_entry = st_entry->scope_next;
        }

        fprintf(stream, "\n");
        head = head->next;
    }
}

void print_table_buckets(SymTable *table, FILE *stream) {
    SymbolTableEntry *st_entry;
    int i;
    for (i = 0; i < table->buckets; i++) {
        fprintf(stream, "-------    Bucket#%d    -------\n", i);
        st_entry = table->HashTable[i];
        while (st_entry != NULL) {
            fprintf(stream, "\"%s\" [%s] (line %d) (scope %d)\n", st_entry->value.funcVal->name, toString(st_entry->type), st_entry->value.funcVal->line, st_entry->value.funcVal->scope);
            st_entry = st_entry->table_next;
        }
    }
}