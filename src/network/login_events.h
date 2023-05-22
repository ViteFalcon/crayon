#include <server_packets.h>

namespace crayon {
class ServerCommandListener;

/// <summary>
/// Responds to successful login packet
/// </summary>
/// <param name="packet">the parsed packet</param>
/// <param name="listener">the listener that should respond to the translated event</param>
void on_login_success(server_packets_t::login_details_payload_t& packet, ServerCommandListener& listener);

/// <summary>
/// Responds to failed login
/// </summary>
/// <param name="packet">the parsed packet</param>
/// <param name="listener">the listener that should resond to the translated event</param>
void on_login_fail(server_packets_t::login_failed_payload_t& packet, ServerCommandListener& listener);

/// <summary>
/// Responds to ban notification
/// </summary>
/// <param name="packet">the parsed packet</param>
/// <param name="listener">the listener that should respond to the translated event</param>
void on_ban_notification(server_packets_t::ban_notification_payload_t& packet, ServerCommandListener& listener);
}  // namespace crayon