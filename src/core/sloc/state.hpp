/**
 * @file state.hpp
 *
 * @brief Define o enum `State`, utilizado para representar os diferentes estados durante a análise de código-fonte.
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
#ifndef STATE_HPP
#define STATE_HPP

#include "../common/aliases.hpp"

/**
 * @enum State
 *
 * @brief Representa os diferentes estados possíveis durante a análise do código-fonte.
 *
 * @details Este enum é utilizado em uma máquina de estados finita para determinar em qual
 *          contexto o analisador está no momento da leitura do código-fonte. Os estados ajudam
 *          a diferenciar entre código executável, comentários e literais.
 */
enum class State : byte
{
  UNDEF,              //!< Estado inicial, quando o estado atual não foi definido.
  CODE,               //!< Estado indicando que estamos dentro do código normal (não literais nem comentários).
  LITERAL,            //!< Estado indicando que estamos dentro de um literal (string ou char).
  ESCAPING,           //!< Estado indicando que estamos dentro de uma sequência de escape em literais.
  BLOCK_COMMENT,      //!< Estado indicando que estamos dentro de um comentário de bloco .
  BLOCK_REG_COMMENT,  //!< Estado indicando que estamos dentro de um comentário de bloco regular.
  BLOCK_DOC_COMMENT,  //!< Estado indicando que estamos dentro de um comentário de bloco de documentação.
  LINE_COMMENT,       //!< Estado indicando que estamos dentro de um comentário de linha.
  LINE_REG_COMMENT,   //!< Estado indicando que estamos dentro de um comentário de linha regular .
  LINE_DOC_COMMENT    //!< Estado indicando que estamos dentro de um comentário de linha de documentação.
};

#endif  //!< STATE_HPP
