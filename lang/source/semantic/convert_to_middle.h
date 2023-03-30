#ifndef aw_script_convert_to_middle_h
#define aw_script_convert_to_middle_h

#include "context.h"

#include <aw/script/ast/declaration.h>
#include <aw/script/ast/middle/declaration.h>

namespace aw::script {

auto convert_to_middle(context& ctx, const ast::module& mod) -> middle::module;

} // namespace aw::script

#endif // aw_script_convert_to_middle_h
