#include "game_enums.h"

namespace crayon {

#define X(v) {::crayon::ClientState::v, #v},
DEFINE_ENUM(ClientState, CLIENT_STATES)
#undef X

#define X(v) {::crayon::GameState::v, #v},
DEFINE_ENUM(GameState, GAME_STATES)
#undef X

#define X(v) {::crayon::ClientMode::v, #v},
DEFINE_ENUM(ClientMode, CLIENT_MODES)
#undef X

#define X(v) {::crayon::CharacterGender::v, #v},
DEFINE_ENUM(CharacterGender, CHARACTER_GENDERS)
#undef X

#define X(v) {::crayon::LoginFailReason::v, #v},
DEFINE_ENUM(LoginFailReason, LOGIN_FAIL_REASONS)
#undef X

#define X(v) {::crayon::BanReason::v, #v},
DEFINE_ENUM(BanReason, BAN_REASONS)
#undef X

#define X(v) {::crayon::GameServerState::v, #v},
DEFINE_ENUM(GameServerState, GAME_SERVER_STATES)
#undef X

#define X(v) {::crayon::GameServerBadge::v, #v},
DEFINE_ENUM(GameServerBadge, GAME_SERVER_BADGES)
#undef X

}  // namespace crayon