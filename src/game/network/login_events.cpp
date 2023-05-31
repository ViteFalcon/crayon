#include <server_packets.h>

#include "server_command_listener.h"

namespace crayon {
GameServerState to_server_state(server_packets_t::server_state_t state) {
  switch (state) {
    case server_packets_t::SERVER_STATE_NORMAL:
      return GameServerState::Normal;
    case server_packets_t::SERVER_STATE_ADULT_ONLY:
      return GameServerState::AdultsOnly;
    case server_packets_t::SERVER_STATE_MAINTENANCE:
      return GameServerState::InMaintenance;
    case server_packets_t::SERVER_STATE_PAY_TO_PLAY:
      return GameServerState::PayToPlay;
    case server_packets_t::SERVER_STATE_PREMIUM:
      return GameServerState::Premium;
  }
  return GameServerState::Normal;
}

GameServerBadge to_server_badge(server_packets_t::server_property_t property) {
  switch (property) {
    case server_packets_t::SERVER_PROPERTY_NEW_SERVER:
      return GameServerBadge::New;
    default:
      break;
  }
  return GameServerBadge::None;
}

void on_login_success(server_packets_t::login_details_payload_t& packet, ServerCommandListener& listener) {
  auto gender = packet.gender() == server_packets_t::GENDER_FEMALE ? CharacterGender::Female : CharacterGender::Male;
  std::vector<CharacterServer> servers;
  if (packet.servers() != nullptr) {
    for (auto& server : *packet.servers()) {
      servers.emplace_back(CharacterServer{
          .config = ClientConfig::for_ipv4_endpoint(server->ip(), server->port()),
          .name = server->name(),
          .state = to_server_state(server->state()),
          .badge = to_server_badge(server->property()),
      });
    }
  }
  listener.on_login_success({
      .account_id = packet.account_id(),
      .session_id = packet.session_id(),
      .login_id = packet.login_id(),
      .gender = gender,
      .web_auth_token = *packet.web_auth_token(),
      .servers = servers,
  });
}

void on_login_fail(server_packets_t::login_failed_payload_t& packet, ServerCommandListener& listener) {
  auto reason = LoginFailReason::Unknown;
  switch (packet.error_code()) {
    case server_packets_t::LOGIN_FAILED_REASON_UNREGISTERED_USER:
      reason = LoginFailReason::UnregisteredUser;
      break;
    case server_packets_t::LOGIN_FAILED_REASON_INCORRECT_PASSWORD:
      reason = LoginFailReason::IncorrectPassword;
      break;
    case server_packets_t::LOGIN_FAILED_REASON_USER_EXPIRED:
      reason = LoginFailReason::UserExpired;
      break;
    case server_packets_t::LOGIN_FAILED_REASON_REJECTED_FROM_SERVER:
      reason = LoginFailReason::RejectedFromServer;
      break;
    case server_packets_t::LOGIN_FAILED_REASON_BLOCKED_BY_GAME_MASTER_TEAM:
      reason = LoginFailReason::BlockedByGameMasterTeam;
      break;
    case server_packets_t::LOGIN_FAILED_REASON_OUTDATED_CLIENT:
      reason = LoginFailReason::OutdatedClient;
      break;
    case server_packets_t::LOGIN_FAILED_REASON_LOGIN_PROHIBITED_UNTIL:
      reason = LoginFailReason::LoginProhibitedUntil;
      break;
    case server_packets_t::LOGIN_FAILED_REASON_SERVER_OVERLOAD:
      reason = LoginFailReason::ServerOverload;
      break;
    case server_packets_t::LOGIN_FAILED_REASON_NO_MORE_ACCOUNTS_MAYBE_CONNECTED_FROM_COMPANY:
      reason = LoginFailReason::NoMoreAccountsMaybeConnectedFromCompany;
      break;
    case server_packets_t::LOGIN_FAILED_REASON_BANNED_BY_DBA:
      reason = LoginFailReason::BannedByDba;
      break;
    case server_packets_t::LOGIN_FAILED_REASON_EMAIL_NOT_CONFIRMED:
      reason = LoginFailReason::EmailNotConfirmed;
      break;
    case server_packets_t::LOGIN_FAILED_REASON_BANNED_BY_GAME_MASTER:
      reason = LoginFailReason::BannedByGameMaster;
      break;
    case server_packets_t::LOGIN_FAILED_REASON_TEMPORARY_BAN_FOR_DB_WORK:
      reason = LoginFailReason::TemporaryBanForDbWork;
      break;
    case server_packets_t::LOGIN_FAILED_REASON_SELF_LOCKED:
      reason = LoginFailReason::SelfLocked;
      break;
    case server_packets_t::LOGIN_FAILED_REASON_UNPERMITTED_GROUP_01:
    case server_packets_t::LOGIN_FAILED_REASON_UNPERMITTED_GROUP_02:
      reason = LoginFailReason::UnpermittedGroup;
      break;
    case server_packets_t::LOGIN_FAILED_REASON_USER_DELETED:
      reason = LoginFailReason::UserDeleted;
      break;
    case server_packets_t::LOGIN_FAILED_REASON_LOGIN_INFORMATION_REMAINS_AT:
      reason = LoginFailReason::LoginInformationRemainsUntil;
      break;
    case server_packets_t::LOGIN_FAILED_REASON_ACCOUNT_LOCKED_FOR_HACKING_INVESTIGATION:
      reason = LoginFailReason::AccountLockedForHackingInvestigation;
      break;
    case server_packets_t::LOGIN_FAILED_REASON_TEMPORARY_BAN_TO_INVESTIGATE_BUG:
      reason = LoginFailReason::TemporaryBanToInvestigateBug;
      break;
    case server_packets_t::LOGIN_FAILED_REASON_CHARACTER_BEING_DELETED_01:
    case server_packets_t::LOGIN_FAILED_REASON_CHARACTER_BEING_DELETED_02:
      reason = LoginFailReason::CharacterBeingDeleted;
      break;
    default:
      break;
  }
  listener.on_login_failed({
      .reason = reason,
      .details = packet.details(),
  });
}

void on_ban_notification(server_packets_t::ban_notification_payload_t& packet, ServerCommandListener& listener) {
  auto reason = BanReason::Unknown;
  switch (packet.reason()) {
    case server_packets_t::BAN_REASON_SERVER_CLOSED:
      reason = BanReason::ServerIsClosed;
      break;
    case server_packets_t::BAN_REASON_SOMEONE_ELSE_LOGGED_IN:
      reason = BanReason::SomeoneElseTriedToLogin;
      break;
    case server_packets_t::BAN_REASON_ALREADY_LOGGED_IN:
      reason = BanReason::YouAreAlreadyLoggedIn;
      break;
    default:
      break;
  }
  listener.on_banned(reason);
}
}  // namespace crayon