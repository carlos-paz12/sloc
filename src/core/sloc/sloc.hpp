/**
 * @file sloc.hpp
 *
 * @brief Define a classe Sloc utilizada para analisar arquivos-fonte e coletar métricas de contagem de linhas.
 *
 * @author José Carlos da Paz Silva (carlos.paz.707@ufrn.edu.br)
 * @author Leandro Andrade (leandro.andrade.401@ufrn.edu.br)
 *
 * @version 0.1
 * @date 2025-05-09
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef SLOC_HPP
#define SLOC_HPP

// STL includes {{{
#include <fstream>  // `std::ifstream`.
// }}}

// Outro includes {{{
#include "aliases.hpp"    // `str`, `umap`
#include "file_info.hpp"  // `FileInfo`
#include "state.hpp"      // `State`
#include "utils.hpp"      // `trim()`
// }}}

class Sloc
{
private:
  State m_current_state{ State::UNDEF };  //!< Estado atual da máquina de estados finita. Inicialmente indefinido (`UNDEF`),
  char m_literal_delimiter{ '\0' };       //!< Delimitador atual de literal (ex: aspas simples `'` ou duplas `"`).
  // * '\0' é um caractere especial com valor zero (0 no código ASCII). É a representação do caractere nulo em C e C++.

  void reset_states()
  {
    transition_to(State::UNDEF);  // [!] Reseta estado padrão.
    m_literal_delimiter = '\0';   // [!] Reseta delimitador de literal padrão.
  }

  void transition_to(State new_state) { m_current_state = new_state; }

  bool handle_escape(const str& token)
  {
    /* [!]
     * Quando o estado atual é `ESCAPING`, significa que o caractere de escape anterior (`\`) já foi processado, então a
     * máquina transita de volta para `LITERAL`, retomando a leitura normal do literal.
     */
    if (m_current_state == State::ESCAPING)
    {
      // [!] Transição de estados: ESCAPING -> \ -> LITERAL.
      transition_to(State::LITERAL);

      /* [!]
       * `return true;` indica que o caractere atual foi tratado como parte de uma sequência de escape, sinalizado que
       * as verificações futuras devem ser ignoradas.
       */
      return true;
    }

    /* [!]
     * Caso o caractere atual seja uma barra invertida (`\`), inicia-se uma sequência de escape, e o estado transita
     * para `ESCAPING` para indicar que o próximo caractere deve ser tratado como parte da sequência.
     */
    if (token.size() >= 2 and token[0] == '\\' and m_current_state == State::LITERAL)
    {
      // [!] Transição de estados: LITERAL -> \ -> ESCAPING.
      transition_to(State::ESCAPING);

      /* [!]
       * Retorna `true` para indicar que o caractere atual foi processado como parte de uma sequência de escape,
       * sinalizado que as verificações futuras devem ser ignoradas.
       */
      return true;
    }

    /* [!]
     * Retorna `false` para indicar que o caractere atual NÃO foi processado como parte de uma sequência de escape,
     * sinalizado para seguir com as verificações.
     */
    return false;
  }

  bool handle_literal(const str& token)
  {
    // [!] Checa se um literal já foi aberto.
    if (m_current_state == State::LITERAL)
    {
      // [!] Se o caractere atual for o delimitador que iniciou o literal (aspas simples ou duplas), isso indica o fim
      // do literal.
      if (token[0] == m_literal_delimiter)
      {
        // [!] Transição de estados: LITERAL -> ' " -> CODE.
        transition_to(State::CODE);
        // [!] Reseta delimitador após o fechamento do literal.
        m_literal_delimiter = '\0';
      }

      /* [!]
       * Retorna `true` para indicar que o caractere foi processado como parte de um literal, sinalizando que as
       * verificações futuras devem ser ignoradas.
       */
      return true;
    }

    // [!] Se ainda não estamos dentro de um literal e o caractere atual for uma aspa, e não estivermos dentro de um
    // bloco de comentário...
    if (not in_block_comment() and (token[0] == '"' or token[0] == '\''))
    {
      // Transição de estados: CODE -> ' " -> LITERAL.
      transition_to(State::LITERAL);
      // [!] Armazena qual foi o delimitador usado (`"` ou `'`) para saber quando o literal terminar.
      m_literal_delimiter = token[0];

      /* [!]
       * Retorna `true` para indicar que o caractere foi processado como parte de um literal, sinalizando que as
       * verificações futuras devem ser ignoradas.
       */
      return true;
    }

    /* [!]
     * Retorna `false` para indicar que o caractere NÃO foi processado como parte de um literal, sinalizado para seguir
     * com as verificações.
     */
    return false;
  }

  bool in_block_comment()
  {
    return m_current_state == State::BLOCK_COMMENT or m_current_state == State::BLOCK_REG_COMMENT or m_current_state == State::BLOCK_DOC_COMMENT;
  }

  bool handle_block_comment(const str& token, size_t& cursor, flag& had_code, flag& had_reg_comment, flag& had_doc_comment)
  {
    // [!] Se já estamos dentro de um comentário de bloco...
    if (in_block_comment())
    {
      if (m_current_state == State::BLOCK_REG_COMMENT)
      {
        had_reg_comment = true;  // [!] Marca que já foi contabilizado.
      }
      else
      {
        had_doc_comment = true;  // [!] Marca que já foi contabilizado.
      }

      // [!] Checa se o token atual encerra o bloco (`*/`).
      if (token.size() >= 2 and token[0] == '*' and token[1] == '/')
      {
        // [!] Transição de estados: BLOCK_DOC_COMMENT -> */ -> UNDEF ou BLOCK_REG_COMMENT -> */ -> UNDEF
        transition_to(State::UNDEF);
        // [!] Avança o cursor em +1 para pular o segundo caractere do token (`/`), já que o loop ainda vai incrementar mais 1 após retornar.
        ++cursor;
      }
      /* [!]
       * Retorna `true` para indicar que o caractere foi processado como parte de um comentário de bloco, sinalizando
       * que as verificações futuras devem ser ignoradas.
       */
      return true;
    }

    // [!] Se ainda não estamos em um literal, e o token indica início de bloco (`/*`)...
    if (m_current_state != State::LITERAL and token.size() >= 2 and token[0] == '/' and token[1] == '*')
    {
      // [!] Se estamos em código, incrementa LOC e realiza transição inicial.
      if (m_current_state == State::CODE)
      {
        had_code = true;  // [!] Marca que já foi contabilizado.
        //  [!] Transição de estados: CODE -> /* -> BLOCK_COMMENT
      }

      // [!] Estado intermediário antes de decidir o tipo de bloco de comentário.
      transition_to(State::BLOCK_COMMENT);  // [!] Transição de estados: UNDEF -> // -> LINE_COMMENT.

      // [!] Verifica se é um comentário de documentação: `/**` ou `/*!`.
      bool is_doc_comment{ token.size() >= 3 and (token[2] == '*' or token[2] == '!') };

      if (not is_doc_comment)
      {
        // [!] Transição de estados: BLOCK_COMMENT -> * ! -> BLOCK_REG_COMMENT
        transition_to(State::BLOCK_REG_COMMENT);
        had_reg_comment = true;  // [!] Marca que já foi contabilizado.
      }
      else
      {
        // [!] Transição de estados: BLOCK_COMMENT -> !=(* !) -> BLOCK_DOC_COMMENT
        transition_to(State::BLOCK_DOC_COMMENT);
        had_doc_comment = true;  // [!] Marca que já foi contabilizado.
      }

      /* [!]
       * Retorna `true` para indicar que o caractere foi processado como parte de um comentário de bloco, sinalizando
       * que as verificações futuras devem ser ignoradas.
       */
      return true;
    }

    /* [!]
     * Retorna `true` para indicar que o caractere NÃO foi processado como parte de um comentário de bloco, sinalizando
     * que as verificações futuras devem ser ignoradas.
     */
    return false;
  }

  bool handle_line_comment(const str& token, flag& had_code, flag& had_reg_comment, flag& had_doc_comment, FileInfo& file)
  {
    bool comment_line_identified{ token.size() >= 2 and token[0] == '/' and token[1] == '/' };

    if (m_current_state != State::LITERAL and comment_line_identified)
    {
      if (m_current_state == State::CODE)  // [!] Checa se o estado anterior era CODE.
      {
        had_code = true;
        //  [!] Transição de estados: CODE -> // -> LINE_COMMENT.
      }

      transition_to(State::LINE_COMMENT);  // [!] Transição de estados: UNDEF -> // -> LINE_COMMENT.

      bool is_doc_comment{ (token.size() >= 3 and (token[2] == '/' or token[2] == '!')) };

      if (not is_doc_comment)
      {
        had_reg_comment = true;
        // [!] Transição de estados: CODE -> // -> LINE_DOC_COMMENT.
        transition_to(State::LINE_DOC_COMMENT);
      }
      else
      {
        had_doc_comment = true;
        // [!] Transição de estados: CODE -> // -> LINE_REG_COMMENT.
        transition_to(State::LINE_REG_COMMENT);
      }

      return true;  // [!] Linha de comentário identificada.
    }

    return false;  // [!] Linha de comentário não identificada.
  }

  void handle_blank_line(const str& line, flag& had_code, flag& had_reg_comment, flag& had_doc_comment, flag& had_blank_line)
  {
    if (line.empty())  // [!] Transição de estados: UNDEF -> Ø -> EMPTY
    {
      if (m_current_state == State::LITERAL)  // [!] Lida com linha vazia dentro de literal.
      {
        // [!] Útil para **raw strings**, onde é possível ter linha vazia entre outras linhas.
        had_code = true;
      }
      else if (in_block_comment())  // [!] Lida com linha vazia dentro de comentário.
      {
        // [!] Verifica qual tipo de comentário de bloco deve ser alterado.
        if (m_current_state == State::BLOCK_REG_COMMENT)
        {
          had_reg_comment = true;
        }
        else
        {
          had_doc_comment = true;
        }
      }
      else  // [!] Lida com linha vazia """pura""".
      {
        had_blank_line = true;
        transition_to(State::UNDEF);  // [!] Transição de estados: EMPTY -> \n -> UNDEF
      }
    }
  }

  void finalize_line_processing(flag& had_code, flag& had_reg_comment, flag& had_doc_comment, flag& had_blank_line, FileInfo& file)
  {
    file.n_loc += static_cast<count_t>(had_code);                  // [!] Atualiza o contador de linhas de código.
    file.n_doc_comments += static_cast<count_t>(had_doc_comment);  // [!] Atualiza o contador de comentários em bloco regulares.
    file.n_reg_comments += static_cast<count_t>(had_reg_comment);  // [!] Atualiza o contador de comentários em bloco de documentação.
    file.n_blank_lines += static_cast<count_t>(had_blank_line);    // [!] Atualiza o contador de linhas vazias.
    file.n_lines++;                                                // [!] Atualiza o contador de linhas totais do arquivo.

    if (m_current_state == State::CODE)  // [!] Reseta o estado atual caso tenhamos encerrado a linha como código.
    {
      m_current_state = State::UNDEF;  // [!] Transição de estados: CODE -> \n -> LITERAL
    }
  }

  /* [!]
   * Cada linha analisada pode ser classificada como uma das seguintes: código, comentário ou vazia. (Em alguns casos, a
   * mesma linha está mutuamente dentro de mais de uma dessas categorias, XD).
   *
   * De toda forma, a análise precisa respeitar uma ordem de prioridade, pois certos elementos podem mascarar outros.
   *
   * O primeiro ponto a ser verificado é se estamos dentro de um literal (LITERAL), já que literais podem conter
   * sequências que se parecem com comentários, como `//` ou `/ *...`, mas que na verdade fazem parte da string.
   *
   * Exemplo:
   *     `std::string str("// Isso não é um comentário real");`
   *
   * Nesse caso, não devemos interpretar o trecho como um comentário, e sim como parte de um literal.
   * Isso nos dá a primeira regra de precedência:
   *     Comentários < Literais
   *
   * Porém, dentro de um literal, é comum encontrar aspas escapadas (`\"` ou `\'`) usadas para representar caracteres
   * especiais. Para tratá-las corretamente, devemos verificar se o caractere atual está escapando outro, antes de
   * decidir se ele está encerrando um literal. Assim, temos mais uma camada de prioridade: Comentários < Literais <
   * Escape
   *
   * A seguir, avaliamos se a linha é vazia. No entanto, linhas vazias podem aparecer dentro de comentários em bloco ou
   * em literais do tipo `raw string`, e por isso não podem ser tratadas de forma isolada. A prioridade se estende:
   *     Linha vazia < Comentários < Literais < Escape
   *
   * Finalmente, se a linha não se encaixa em nenhuma das categorias anteriores, consideramos que ela contém código
   * executável.
   *
   * A ordem de análise final, do menor para o maior nível de prioridade, é:
   *     Código < Linha vazia < Comentários < Literais < Escape
   */
  void process_line(const str& line, FileInfo& file)
  {
    const str trimmed_line{ trim(line) };  //!< Linha atual sem espaços em branco no início e fim.

    flag had_code{ false };         //!< Flag para indicar se já foi encontrado código nesta linha.
    flag had_reg_comment{ false };  //!< Flag para indicar se já foi encontrado comentário regular nesta linha.
    flag had_doc_comment{ false };  //!< Flag para indicar se já foi encontrado comentário de documentação nesta linha.
    flag had_blank_line{ false };   //!< Flag para indicar se linha foi considerada vazia.

    /* [!]
     * A verificação de linha vazia precisa ocorrer antes do loop de análise.
     * Isso porque, após o `trim`, linhas contendo apenas espaços serão reduzidas a uma string vazia.
     * Se esse tratamento fosse feito *dentro* do loop, ele não seria executado, pois `trimmed_line.size()` seria zero.
     */
    handle_blank_line(trimmed_line, had_code, had_reg_comment, had_doc_comment, had_blank_line);

    // [!] Percorre caractere por caractere da linha atual.
    for (std::size_t cursor{ 0 }; cursor < trimmed_line.size(); ++cursor)
    {
      // [!] Define o tamanho do token atual como, no máximo, 3 caracteres (ou o restante da linha, se menor).
      //     Isso permite capturar padrões multicaractere (como `//`, `/*`, `*/`) sem ultrapassar os limites da string.
      const std::size_t token_size{ std::min(size_t{ 3 }, trimmed_line.size() - cursor) };
      // [!] Extrai o token a partir da posição atual do cursor, com o tamanho definido.
      //     Esse token será usado nas verificações de escape, literais e comentários.
      const str token{ trimmed_line.substr(cursor, token_size) };

      // [!] #1 Lida com caracteres de escape.
      if (handle_escape(token))
      {
        continue;  // [!] Caractere de escape identificado, ignora próximas verificações.
      }

      // [!] #2 Lida com literais.
      if (handle_literal(token))
      {
        had_code = true;
        continue;  // [!] Literal identificado, ignora próximas verificações.
      }

      // [!] #3 Lida com blocos de comentário.
      if (handle_block_comment(token, cursor, had_code, had_reg_comment, had_doc_comment))
      {
        continue;  // [!] Bloco de comentário indentificado, ignora próximas verificações.
      }

      // [!] #4 Lida com linhas de comentário.
      if (handle_line_comment(token, had_code, had_reg_comment, had_doc_comment, file))
      {
        transition_to(State::UNDEF);  // [!] Transição de estados: LINE_DOC_COMMENT -> \n -> UNDEF ou LINE_REG_COMMENT -> \n -> UNDEF.
        /* [!]
         * Se uma linha de comentário foi identificada, o `break` encerra a verificação da linha atual,
         * independentemente da posição do cursor. Isso ocorre porque, a partir desse ponto, todo o restante da linha
         * será considerado comentário.
         */
        break;  // [!] Linha de comentário identificada.
      }

      /* [!]
       * Espaços em branco isolados não representam código e não devem acionar transição para `CODE`.
       * Do contrário, espaços entre dois blocos de comentário poderiam ser erroneamente contados como linhas de código.
       */
      if ((std::isspace(token[0]) == 0) and not in_block_comment() and not had_code)
      {
        // [!] Transição de estados: UNDEF -> !=(Ø, //, /*) -> CODE
        transition_to(State::CODE);  // [!] #5 O que não for vazio, nem comentário, e não for um espaço em branco, é visto como código.
        had_code = true;
      }
    }

    // [!] Finaliza o processamento da linha atual.
    finalize_line_processing(had_code, had_reg_comment, had_doc_comment, had_blank_line, file);
  }

  void read_and_process(FileInfo& file)
  {
    // [!] Abre o arquivo de entrada com o nome armazenado em `file.filename`.
    std::ifstream ifs{ file.m_filename };

    // [!] Verifica se o arquivo foi aberto com sucesso.
    if (ifs.is_open())
    {
      str line{};  //!< Buffer para armazenar a linha atual sendo lida do arquivo.

      // [!] Reinicia os estados da máquina antes de começar o processamento.
      reset_states();

      // [!] Lê o arquivo linha por linha até o final (EOF).
      while (std::getline(ifs, line))
      {
        process_line(line, file);  // [!] Processa a linha atual usando a máquina de estados.
      }
    }

    ifs.close();  // [!] Fecha o arquivo após a leitura.
  }

public:
  void analyze_file(FileInfo& file)
  {
    read_and_process(file);  // [!] Inicia leitura e análise linha a linha do arquivo.
  }
};

#endif  //!< SLOC_HPP
