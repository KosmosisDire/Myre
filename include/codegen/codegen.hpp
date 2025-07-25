#pragma once

#include "ast/ast.hpp"
#include "semantic/symbol_table.hpp"
#include "codegen/ir_builder.hpp"
#include "codegen/ir_command.hpp"
#include <memory>
#include <unordered_map>
#include <vector>

namespace Mycelium::Scripting::Lang {

struct VariableInfo {
    ValueRef value_ref;
    IRType type;
};

class CodeGenerator : public StructuralVisitor {
private:
    SymbolTable& symbol_table_;
    std::unique_ptr<IRBuilder> ir_builder_;
    std::unordered_map<std::string, VariableInfo> local_vars_;
    ValueRef current_value_;  // Result of last expression

public:
    CodeGenerator(SymbolTable& table);
    ~CodeGenerator() = default;

    // Basic visitor overrides for initial implementation
    void visit(CompilationUnitNode* node) override;
    void visit(LiteralExpressionNode* node) override;
    void visit(BinaryExpressionNode* node) override;
    void visit(UnaryExpressionNode* node) override;
    void visit(VariableDeclarationNode* node) override;
    void visit(IdentifierExpressionNode* node) override;
    void visit(ReturnStatementNode* node) override;
    void visit(FunctionDeclarationNode* node) override;
    void visit(TypeDeclarationNode* node) override;
    void visit(BlockStatementNode* node) override;
    void visit(AssignmentExpressionNode* node) override;
    void visit(IfStatementNode* node) override;
    void visit(WhileStatementNode* node) override;
    void visit(ForStatementNode* node) override;
    void visit(ExpressionStatementNode* node) override;
    void visit(CallExpressionNode* node) override;
    void visit(MemberAccessExpressionNode* node) override;
    void visit(IndexerExpressionNode* node) override;
    void visit(NewExpressionNode* node) override;

    // Generate code from AST and return command list
    std::vector<Command> generate_code(CompilationUnitNode* root);

private:
    // Helper to build struct layout from symbol table
    std::shared_ptr<StructLayout> build_struct_layout(const std::string& struct_name);
    
    // Pre-generate all struct types from symbol table
    void pre_generate_struct_types();
    
    // Helper to find field index in struct layout
    int find_field_index(const StructLayout& layout, const std::string& field_name);
    
    // Helper to generate member functions with implicit 'this' parameter
    void visit_member_function(FunctionDeclarationNode* node, const std::string& owner_type);
};

} // namespace Mycelium::Scripting::Lang