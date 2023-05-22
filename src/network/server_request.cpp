#include "server_request.h"

namespace crayon {
#define X(Name, Value) {::crayon::ServerRequest::Name, #Name},
DEFINE_ENUM(ServerRequest, SERVER_REQUESTS)
#undef X
}  // namespace crayon