#ifndef SYMBOL_H_INCLUDED
#define SYMBOL_H_INCLUDED

#include "ast.h"

#include <vector>
#include <forward_list>
#include <unordered_map>
#include <memory>

namespace ST{

class SymbolNode;

class SymbolTable {
    private:
        std::vector<std::unique_ptr<SymbolNode>> m_symbolNodes;     // Ownership of all SymbolNodes
    private:
        std::forward_list<SymbolNode *> m_scope;                    // Scopes
        std::vector<SymbolNode *> m_symbolTreeScopeLeaves;          // Symbol Tree Scope Leaves
        bool m_encounterNewScope = false;                           // Scope flag
        SymbolNode *m_currentHead = nullptr;                        // Current SymbolNode
    private:
        std::unordered_map<AST::IdentifierNode *, SymbolNode *> m_positionOfRef;
                                                                    // Position of reference of Identifier
    private:
        FILE *m_out = stdout;

    public:
        SymbolTable();
        ~SymbolTable();

    public:
        void clear();

    public:
        /* Declaration Related */
        void enterScope();
        void exitScope();
        AST::DeclarationNode *declareSymbol(AST::DeclarationNode *decl);
    
    public:
        /* Identifier Related */
        void markSymbolRefPos(AST::IdentifierNode *ident);
        AST::DeclarationNode *getSymbolDecl(AST::IdentifierNode *ident) const;
        AST::DeclarationNode *findAnyRedeclaration(AST::DeclarationNode *decl) const;
    
    public:
        void printScopeLeaves() const;
        void printSymbolReference() const;
        void setOutput(FILE *out) { m_out = out; }
        FILE *getOutput() const { return m_out; }

    private:
        /* Helper Functions */
        /* Return redecl if redeclaration under current scope; otherwise nullptr */
        AST::DeclarationNode *findRedeclaration(AST::DeclarationNode *decl) const;
        static bool checkSymbolMatch(AST::DeclarationNode *decl, AST::IdentifierNode *ident);
        int printSymbolTreeTo(SymbolNode *node, const AST::DeclarationNode *markDecl = nullptr) const;
};

}

#endif