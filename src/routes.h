#include "app/router/router.h"
#include "app/session/session.h"
std::shared_ptr<const Router>
get_routes(std::shared_ptr<SessionStore> session_store);
