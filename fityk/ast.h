// This file is part of fityk program. Copyright 2001-2013 Marcin Wojdyr
// Licence: GNU General Public License ver. 2+

#ifndef FITYK_AST_H_
#define FITYK_AST_H_

#include <assert.h>
#include "vm.h" // opcodes
#include "common.h" // DISALLOW_COPY_AND_ASSIGN

namespace fityk {

struct OpTreeFormat
{
    const char *num_format;
    const std::vector<std::string> *vars;
};

/// Node in abstract syntax tree (AST)
struct OpTree
{
    /// op < 0: variable (n=-op-1)
    /// op == 0 (== OP_NUMBER): constant
    /// op > 0: operator
    /// There is no OP_X in OpTree, x (if there is x) is the last variable
    int op;
    OpTree *c1,
           *c2;
    realt val;

    explicit OpTree(realt v) : op(0), c1(0), c2(0), val(v) {}

    explicit OpTree(void*, int n) : op(-1-n), c1(0), c2(0), val(0.) {}

    explicit OpTree(int n, OpTree *arg1) : op(n), c1(arg1), c2(0), val(0.)
                              { assert(n >= OP_ONE_ARG && n < OP_TWO_ARG); }

    explicit OpTree(int n, OpTree *arg1, OpTree *arg2)
        : op(n), c1(arg1), c2(arg2), val(0.)   { assert(n >= OP_TWO_ARG); }

    ~OpTree() { delete c1; delete c2; }

    std::string str(const OpTreeFormat& fmt);
    std::string str_b(bool b, const OpTreeFormat& fmt)
                            { return b ? "(" + str(fmt) + ")" : str(fmt); }
    OpTree* clone() const;
    //void swap_args() { assert(c1 && c2); OpTree *t=c1; c1=c2; c2=t; }
    OpTree* remove_c1() { OpTree *t=c1; c1=0; return t; }
    OpTree* remove_c2() { OpTree *t=c2; c2=0; return t; }
    void change_op(int op_) { op=op_; }
    bool operator==(const OpTree &t) const {
        return op == t.op && val == t.val
               && (c1 == t.c1 || (c1 && t.c1 && *c1 == *t.c1))
               && (c2 == t.c2 || (c2 && t.c2 && *c2 == *t.c2));
    }
private:
    DISALLOW_COPY_AND_ASSIGN(OpTree);
};

std::vector<OpTree*> prepare_ast_with_der(const VMData& vm, int len);
std::string simplify_formula(const std::string &formula, const char* num_fmt);
void get_derivatives_str(const char* formula, std::string& result);
void add_bytecode_from_tree(const OpTree* tree,
                            const std::vector<int> &symbol_map, VMData& vm);
} // namespace fityk
#endif

