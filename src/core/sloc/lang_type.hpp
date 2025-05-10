/**
 * @file lang_type.hpp
 *
 * @brief Define os tipos de linguagem de programação suportados.
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
#ifndef LANG_TYPE_HPP
#define LANG_TYPE_HPP

#include "aliases.hpp"  // `byte`

/**
 * @enum LangType
 *
 * @brief Enum class que representa as linguagens de programação suportadas.
 */
enum class LangType : byte
{
  C = 0,  //!< Código fonte C.
  H,      //!< Cabeçalho C/C++.
  CPP,    //!< Código fonte C++.
  HPP,    //!< Cabeçalho C++.
  UNDEF,  //!< Tipo indefinido.
};

#endif  //!< LANG_TYPE_HPP
