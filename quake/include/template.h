#ifndef _QUAKE_TEMPLATE_H_
#define _QUAKE_TEMPLATE_H_
#include "base.h"

// A simple template file used to help performing verification and other general
// tasks. Each template file just contains 2 types of elements. A textual blob
// with tagging, ie mark what does the blob means; and also optional comments

namespace quake {

struct Template {
  struct Section {
    std::string tag;
    std::string text;
  };
  std::vector<Section> content;

  std::optional<std::string_view> GetSection(const std::string_view&) const;
};

base::Result<Template> ParseTemplate(const std::string_view&);
base::Result<Template> ParseTemplateFile(const std::string_view&);

}  // namespace quake

#endif  // _QUAKE_TEMPLATE_H_

