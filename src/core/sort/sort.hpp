/**
@file sort.hpp
 * @author José Carlos da Paz Silva (carlos.paz.707@ufrn.edu.br)
 * @author Leandro Andrade (leandro.andrade.401@ufrn.edu.br)
 *
 * @brief Classe para ordenação de arquivos.
 * @version 0.1
 * @date 2025-05-10
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef SORT_HPP
#define SORT_HPP

#include <algorithm>
#include <functional>

#include "../common/aliases.hpp"
#include "../core/filter/field_option.hpp"
#include "../core/options/running_options.hpp"
#include "../core/sloc/file_info.hpp"

//==================================================Aliases==================================================//
using func = std::function<bool(const FileInfo&, const FileInfo&)>;
//=============================================================================================================//

class Sort
{
public:
  /**
   * @brief  Ordena os arquivos com base no campo especificado. A ordem de
   *  Ordenação é determinada pelo parâmetro `ascending` passado na estrutura
   *  `RunningOptions`. Caso o campo não deseja ativada, a ordem de
   *  ordenação padrão é decrescente.
   *
   * @details A função utiliza um mapa para associar cada campo a uma função de comparação
   *  apropriada. A função de comparação é então usada para ordenar o vetor de arquivos.
   *  A função de comparação é definida como uma função lambda que compara dois objetos
   *  `FileInfo` com base no campo especificado. A função de comparação retorna verdadeiro
   *  se o primeiro objeto deve vir antes do segundo na ordenação, com base no campo
   *  especificado e na ordem de classificação (crescente ou decrescente).
   *  A função `sortSloc` é um método estático que pode ser chamado sem a necessidade de
   *  criar uma instância da classe `Sort`.
   *
   * @param files  Objeto do tipo `vec<FileInfo>` que contém os arquivos a serem ordenados.
   *
   * @param option  Campo de ordenação especificado pelo usuário. Pode ser um dos seguintes:
   *  - `FieldOption::FILENAME`: ordena por nome de arquivo.
   *  - `FieldOption::FILETYPE`: ordena por tipo de arquivo.
   *  - `FieldOption::SLOC`: ordena por número de linhas de código.
   *  - `FieldOption::COMMENTS`: ordena por número de comentários regulares.
   *  - `FieldOption::DOC_COMENTS`: ordena por número de comentários de documentação.
   *  - `FieldOption::BLANK_LINES`: ordena por número de linhas em branco.
   *  - `FieldOption::ALL`: ordena por número total de linhas.
   *
   * @param ro  Estrutura `RunningOptions` que contém as opções de execução, incluindo
   *  a ordem de classificação (crescente ou decrescente) e o campo de ordenação.
   */
  static void sortSloc(vec<FileInfo>& files, const FieldOption option, const RunningOptions& ro)
  {
    //[!] Mapa de comparação para cada campo de ordenação.
    // [!] O mapa associa cada campo a uma função de comparação apropriada.
    umap<FieldOption, func> sorting_compare = { { FieldOption::FILENAME, cmp(&FileInfo::m_filename, ro.ascending) },
                                                { FieldOption::FILETYPE, cmp(&FileInfo::m_type, ro.ascending) },
                                                { FieldOption::SLOC, cmp(&FileInfo::n_loc, ro.ascending) },
                                                { FieldOption::COMMENTS, cmp(&FileInfo::n_reg_comments, ro.ascending) },
                                                { FieldOption::DOC_COMENTS, cmp(&FileInfo::n_doc_comments, ro.ascending) },
                                                { FieldOption::BLANK_LINES, cmp(&FileInfo::n_blank_lines, ro.ascending) },
                                                { FieldOption::ALL, cmp(&FileInfo::n_lines, ro.ascending) } };

    if (sorting_compare.count(option))  //[!] verifica se o campo de ordenação é valido
    {
      std::sort(files.begin(), files.end(), sorting_compare[option]);  // [!] Ordena os arquivos com base no campo de ordenação.
    }
  }

private:
  /**
   * @brief Função de comparação para ordenar arquivos com base em um campo específico.
   *
   * @details Esta função é um método estático que retorna uma função de comparação
   *  apropriada para ordenar arquivos com base em um campo específico. O campo é
   *  passado como um ponteiro para membro da estrutura `FileInfo`. A função de
   *  comparação retornada compara dois objetos `FileInfo` com base no campo
   *  especificado e na ordem de classificação (crescente ou decrescente).
   *
   * @param field  Campo a ser comparado. É um ponteiro para membro da estrutura
   *  `FileInfo` que representa o campo a ser usado na comparação.
   *
   * @param ascending  Indica se a ordenação deve ser crescente (`true`) ou
   *  decrescente (`false`).
   *
   * @return func retorna uma função de comparação que pode ser usada para ordenar arquivos
   *  com base no campo especificado e na ordem de classificação.
   */
  template <typename Type>  //[!] template para permitir a comparação de diferentes tipos de campos.
  static func cmp(Type FileInfo::*field, bool ascending)
  {
    // [!] Retorna uma função de comparação que compara dois objetos FileInfo com base no campo especificado.
    return [=](const FileInfo& a, const FileInfo& b) { return ascending ? a.*field < b.*field : a.*field > b.*field; };
  }
};

#endif  // SORT_HPP
