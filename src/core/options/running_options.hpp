/**
 * @file running_options.hpp
 *
 * @brief Define a estrutura RunningOptions utilizada para tratar os argumentos de linha de comando.
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
#ifndef RUNNING_OPTIONS_HPP
#define RUNNING_OPTIONS_HPP

#include "aliases.hpp"       // `option`, `vec`
#include "field_option.hpp"  // `FieldOption`
#include "file_info.hpp"     // `FileInfo`

/**
 * @struct RunningOptions
 *
 * @brief Struct para armazenar opções de linha de comando.
 *
 * @details RunningOptions contém os valores analisados dos argumentos da linha de comando que
 *          determinam o comportamento da aplicação. Ela inclui opções para análise recursiva,
 *          preferências de ordenação e os arquivos a processar.
 */
struct RunningOptions
{
  option recursive{ false };                    //!< Sinalizador de análise recursiva (predefinição: false).
  option ascending{ false };                    //!< Sinalizador de tipo de ordenação (default: descendente).
  FieldOption sort_field{ FieldOption::NONE };  //!< Campo de ordenação para a saída da tabela.
  vec<FileInfo> sources;                        //!< Lista de arquivos de código fonte.
};

#endif  //!< RUNNING_OPTIONS_HPP
