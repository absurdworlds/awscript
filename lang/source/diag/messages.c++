#include <aw/script/diag/diagnostic.h>

namespace aw::script {

std::string diag_message_template(diagnostic_id id)
{
	switch (id) {
#define DIAG(type, id, msg) case diagnostic_id::id: return msg;
#include <aw/script/diag/messages.h>
#undef DIAG
	}
	return {};
}
} // namespace aw::script
