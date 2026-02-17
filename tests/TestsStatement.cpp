#include "TestsStatement.h"
#include "../src/lexer/Token/Token.h"
#include "../src/parser/ASTNodes/Statement.h"
#include "../src/utils/Assert.h"

void testStatement() {
  Token nameVar(TokenType::IDENTIFIER, "v", 1);
  Token num100(TokenType::NUMBER_TOKEN, "100", 1);
  LiteralExpression lit100(num100);

  // VarDeclarationStatement: v = 100
  VarDeclarationStatement varDecl1(nameVar, new LiteralExpression(num100));
  GW_ASSERT(varDecl1.getName().getValue() == "v");
  GW_ASSERT(varDecl1.getInitializer()->getType() == ExpressionType::LITERAL);

  // VarDeclarationStatement: v (no initializer)
  VarDeclarationStatement varDecl2(nameVar);
  GW_ASSERT(varDecl2.getName().getValue() == "v");
  GW_ASSERT(varDecl2.getInitializer()->getType() == ExpressionType::EMPTY_EXPR);

  // ExpressionStatement
  ExpressionStatement exprStmt(new LiteralExpression(num100));
  GW_ASSERT(exprStmt.getExpression()->getType() == ExpressionType::LITERAL);

  // PrintStatement
  PrintStatement printStmt(new LiteralExpression(num100));
  GW_ASSERT(printStmt.getExpression()->getType() == ExpressionType::LITERAL);

  // BlockStatement
  List<Statement *> stmts;
  stmts.append(
      new VarDeclarationStatement(nameVar, new LiteralExpression(num100)));
  stmts.append(new PrintStatement(new LiteralExpression(num100)));
  BlockStatement block(stmts);
  GW_ASSERT(block.getStatements().size() == 2);

  // IfStatement (If-Then-Else)
  Token trueTok(TokenType::BOOLEAN_TOKEN, "true", 1);
  IfStatement ifElse(new LiteralExpression(trueTok),
                     new PrintStatement(new LiteralExpression(num100)),
                     new ExpressionStatement(new LiteralExpression(num100)));
  GW_ASSERT(ifElse.getCondition()->getType() == ExpressionType::LITERAL);

  // WhileStatement
  List<Statement *> blockStmts;
  blockStmts.append(new PrintStatement(new LiteralExpression(num100)));
  WhileStatement whileStmt(new LiteralExpression(trueTok),
                           new BlockStatement(blockStmts));
  GW_ASSERT(whileStmt.getCondition()->getType() == ExpressionType::LITERAL);

  // ForStatement: for(i; i < 10; i++)
  ForStatement forStmt(new ExpressionStatement(new LiteralExpression(num100)),
                       new LiteralExpression(trueTok),
                       new LiteralExpression(num100),
                       new PrintStatement(new LiteralExpression(num100)));
  GW_ASSERT(forStmt.getInitializer()->getType() ==
            StatementType::EXPRESSION_STATEMENT);

  // FunctionDeclarationStatement (Int result, no params)
  List<List<Token>> emptyParams;
  List<Statement *> funcStmts;
  funcStmts.append(new PrintStatement(new LiteralExpression(num100)));
  FunctionDeclarationStatement func1(nameVar, emptyParams,
                                     new BlockStatement(funcStmts),
                                     DataTypes::Int, false);
  GW_ASSERT(func1.getReturnType() == DataTypes::Int);
  GW_ASSERT(func1.getParams().size() == 0);

  // ReturnStatement
  ReturnStatement retVal(new LiteralExpression(num100));
  GW_ASSERT(retVal.getValue()->getType() == ExpressionType::LITERAL);

  ReturnStatement retEmpty;
  GW_ASSERT(retEmpty.getValue() == nullptr);

  // StructDeclarationStatement
  List<List<Token>> fields;
  List<Token> field1;
  field1.append(Token(TokenType::INT_TOKEN, "int", 1));
  field1.append(Token(TokenType::IDENTIFIER, "age", 1));
  fields.append(field1);
  StructDeclarationStatement structDecl(
      Token(TokenType::IDENTIFIER, "Person", 1), fields);
  GW_ASSERT(structDecl.getFields().size() == 1);
  GW_ASSERT(structDecl.getName().getValue() == "Person");

  // ClassDeclarationStatement
  List<FunctionDeclarationStatement *> emptyMethods;
  List<ConstructorDeclarationStatement *> emptyConstructors;
  List<OperatorDeclarationStatement *> emptyOperators;
  ClassDeclarationStatement classDecl(
      Token(TokenType::IDENTIFIER, "MyClass", 1), Token(), fields, emptyMethods,
      emptyConstructors, emptyOperators);
  GW_ASSERT(classDecl.getName().getValue() == "MyClass");

  // Operators, Break, Continue
  List<Statement *> opStmts;
  opStmts.append(new PrintStatement(new LiteralExpression(num100)));
  OperatorDeclarationStatement opDecl(Token(TokenType::MATH_OPERATOR, "+", 1),
                                      emptyParams, new BlockStatement(opStmts),
                                      DataTypes::Int);
  GW_ASSERT(opDecl.getOperator().getValue() == "+");

  BreakStatement b;
  ContinueStatement c;
}
