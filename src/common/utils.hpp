/**
 * @file utils.hpp
 *
 * @brief Fornece funções utilitárias utilizadas no projeto.
 *
 * @author José Carlos da Paz Silva (carlos.paz.707@ufrn.edu.br)
 * @author Leandro Andrade (leandro.andrade.401@ufrn.edu.br)
 *
 * @version 0.1
 * @date 2025-05-07
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef UTILS_HPP
#define UTILS_HPP

#include "aliases.hpp"    // `str`
#include "constants.hpp"  // `WHITESPACE`

/**
 * @brief Remove os espaços em branco à esquerda da string fornecida.
 *
 * @param input A string a ser processada.
 *
 * @return Uma nova string com os espaços em branco removidos à esquerda.
 */
inline str ltrim(const str& input)
{
  // [!] Encontra o índice do primeiro caractere que não seja espaço em branco, começando do início da string.
  const auto start{ input.find_first_not_of(WHITESPACE) };

  /* [!]
   * Se não encontrar nenhum caractere não-espaço (string toda é espaço), retorna uma string vazia.
   * Caso contrário, retorna a substring começando do primeiro caractere não-espaço.
   */
  return (start == str::npos) ? "" : input.substr(start);
}

/**
 * @brief Remove os espaços em branco à direita da string fornecida.
 *
 * @param input A string a ser processada.
 *
 * @return Uma nova string com os espaços em branco removidos à direita.
 */
inline str rtrim(const str& input)
{
  // [!] Encontra o índice do último caractere que não seja espaço em branco, começando do início da string.
  const auto end{ input.find_last_not_of(WHITESPACE) };

  /* [!]
   * Se não encontrar nenhum caractere não-espaço (string toda é espaço), retorna uma string vazia.
   * Caso contrário, retorna a substring começando do primeiro caractere não-espaço.
   */
  return (end == str::npos) ? "" : input.substr(0, end + 1);
}

/**
 * @brief Remove os espaços em branco à esquerda e à direita da string fornecida.
 *
 * @param input A string a ser processada.
 *
 * @return Uma nova string com os espaços em branco removidos de ambos os lados.
 *
 * @note Aplica `ltrim`, depois `rtrim` sobre @a input.
 */
inline str trim(const str& input) { return rtrim(ltrim(input)); }

#endif  //!< UTILS_HPP
