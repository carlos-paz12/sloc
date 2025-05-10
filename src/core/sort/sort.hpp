#ifndef SORT_HPP
#define SORT_HPP

#include <algorithm>
#include <functional>
#include <string>
#include "aliases.hpp"
#include "field_option.hpp"
#include "file_info.hpp"
#include "lang_type.hpp"
#include "running_options.hpp"

using func = std::function<bool(const FileInfo&, const FileInfo&)>;

class Sort
{
public:
  static void sortSloc(vec<FileInfo>& files, const FieldOption option, const RunningOptions& ro)
  {
    umap<FieldOption, func> sorting_compare = { { FieldOption::FILENAME, cmp(&FileInfo::m_filename, ro.ascending) },
                                                { FieldOption::FILETYPE, cmp(&FileInfo::m_type, ro.ascending) },
                                                { FieldOption::SLOC, cmp(&FileInfo::n_loc, ro.ascending) },
                                                { FieldOption::COMMENTS, cmp(&FileInfo::n_reg_comments, ro.ascending) },
                                                { FieldOption::DOC_COMENTS, cmp(&FileInfo::n_doc_comments, ro.ascending) },
                                                { FieldOption::BLANK_LINES, cmp(&FileInfo::n_blank_lines, ro.ascending) },
                                                { FieldOption::ALL, cmp(&FileInfo::n_lines, ro.ascending) } };

    if (sorting_compare.count(option))
    {
      std::sort(files.begin(), files.end(), sorting_compare[option]);
    }
  }

private:
  template <typename Type>
  static func cmp(Type FileInfo::*field, bool ascending)
  {
    return [=](const FileInfo& a, const FileInfo& b) { return ascending ? a.*field < b.*field : a.*field > b.*field; };
  }
};

#endif  // SORT_HPP
