/**
 * @file aliases.hpp
 *
 * @brief Fornece aliases de tipos comuns e includes padrão.
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
#ifndef ALIASES_HPP
#define ALIASES_HPP

// STL includes {{{
#include <cstddef>
#include <cstdint>        // `std::uint8_t`
#include <list>           // `std::list`
#include <sstream>        // ``std::ostringstream
#include <string>         // `std::string`
#include <vector>         // `std::vector`
#include <string_view>    // `std::string_view`
#include <unordered_map>  // `std::unordered_map`
// }}}

// Aliases {{{
// ========== Tipos Primitivos ==========
/// @brief Alias para opções de configuração booleanas.
using option = bool;

/// @brief Alias para flags booleanas.
using flag = bool;

/// @brief Alias para representação um único byte de dados.
using byte = std::uint8_t;

/// @brief Alias para representação de comprimento.
using size_t = std::size_t;

/// @brief Alias para `std::string`.
using str = std::string;

/// @brief Alias para `std::string_view`.
using str_view = std::string_view;

/// @brief Alias para `std::ostringstream`.
using oss = std::ostringstream;

// ========== Containers ==========
/// @brief Alias com template para `std::list`.
template <typename Type>
using list = std::list<Type>;

/// @brief Alias com template para `std::unordered_map`.
template <typename Key, typename Type>
using umap = std::unordered_map<Key, Type>;

/// @brief Alias com template para `std::vector`.
template <typename Type>
using vec = std::vector<Type>;
// }}}

#endif  //!< ALIASES_HPP
