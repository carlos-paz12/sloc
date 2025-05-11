/**
 * @file field_option.hpp
 *
 * @brief Define as opções de ordenação suportadas para a saída da tabela SLOC.
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
#ifndef FIELD_OPTION_HPP
#define FIELD_OPTION_HPP

#include "../common/aliases.hpp"  // `byte`

/**
 * @enum FieldOption
 *
 * @brief Enum class que representa as opções de ordenação disponíveis para a saída da tabela SLOC.
 *
 * @details Estas opções especificam os critérios pelos quais a saída do SLOC pode ser ordenada.
 *          Cada opção corresponde a uma coluna ou atributo específico na tabela gerada.
 */
enum class FieldOption : byte
{
  NONE,         //!< Sem ordenação.
  FILENAME,     //!< Ordenar pelo nome do arquivo.
  FILETYPE,     //!< Ordenar pelo tipo do arquivo.
  COMMENTS,     //!< Ordenar pela quantidade de comentários regulares.
  DOC_COMENTS,  //!< Ordenar pela quantidade de comentários de documentação.
  BLANK_LINES,  //!< Ordenar pela quantidade de linhas vazias.
  SLOC,         //!< Ordenar pela quantidade de linhas de código.
  ALL,          //!< Ordenar pela quantidade de linhas totais.
};

#endif  //!< FIELD_OPTION_HPP
