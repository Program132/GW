#include "../src/lexer/Token/Token.h"
#include "../src/lexer/Token/TokenType.h"
#include "../src/utils/Assert.h"

void testToken() {
  Token token(TokenType::IDENTIFIER, "test", 1);
  GW_ASSERT(token.getValue() == "test");
  GW_ASSERT(token.getType() == TokenType::IDENTIFIER);
  GW_ASSERT(token.getLine() == 1);

  token.addLine();
  GW_ASSERT(token.getLine() == 2);

  token.appendCharacter('a');
  GW_ASSERT(token.getValue() == "testa");

  token.appendString("bc");
  GW_ASSERT(token.getValue() == "testabc");

  token.setType(TokenType::INT);
  GW_ASSERT(token.getType() == TokenType::INT);

  token.setValue("test");
  GW_ASSERT(token.getValue() == "test");

  token.setLine(1);
  GW_ASSERT(token.getLine() == 1);

  Token token2(TokenType::INT, "1236545644564", 1);
  GW_ASSERT(token2.getValue() == "1236545644564");
  GW_ASSERT(token2.getType() == TokenType::INT);
  GW_ASSERT(token2.getLine() == 1);

  Token token3(TokenType::BOOLEAN,
               "zaekahjehejkaehkzaehzkjaehaehehzakezaehzakehakekaehzajkehakeh",
               1);
  GW_ASSERT(token3.getValue() ==
            "zaekahjehejkaehkzaehzkjaehaehehzakezaehzakehakekaehzajkehakeh");
  GW_ASSERT(token3.getType() == TokenType::BOOLEAN);
  GW_ASSERT(token3.getLine() == 1);

  Token token4(TokenType::STRING, "test", 1645464654);
  GW_ASSERT(token4.getValue() == "test");
  GW_ASSERT(token4.getType() == TokenType::STRING);
  GW_ASSERT(token4.getLine() == 1645464654);
}