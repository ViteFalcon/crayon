#pragma once

#include "enum_utils.h"

namespace crayon {

#define CLIENT_STATES \
  X(Start)            \
  X(AuthServerSelect) \
  X(Auth)             \
  X(GameServerSelect) \
  X(CharacterSelect)  \
  X(LoadingMap)       \
  X(InGame)           \
  X(Stopped)

#define GAME_STATES   \
  X(Start)            \
  X(AuthServerSelect) \
  X(Auth)             \
  X(Stopped)

#define CLIENT_MODES \
  X(Auth)            \
  X(Character)       \
  X(Map)

#define CHARACTER_GENDERS \
  X(Female)               \
  X(Male)

#define LOGIN_FAIL_REASONS                   \
  X(UnregisteredUser)                        \
  X(IncorrectPassword)                       \
  X(UserExpired)                             \
  X(RejectedFromServer)                      \
  X(BlockedByGameMasterTeam)                 \
  X(OutdatedClient)                          \
  X(LoginProhibitedUntil)                    \
  X(ServerOverload)                          \
  X(NoMoreAccountsMaybeConnectedFromCompany) \
  X(BannedByDba)                             \
  X(EmailNotConfirmed)                       \
  X(BannedByGameMaster)                      \
  X(TemporaryBanForDbWork)                   \
  X(SelfLocked)                              \
  X(UnpermittedGroup)                        \
  X(UserDeleted)                             \
  X(LoginInformationRemainsUntil)            \
  X(AccountLockedForHackingInvestigation)    \
  X(TemporaryBanToInvestigateBug)            \
  X(CharacterBeingDeleted)                   \
  X(Unknown)

#define BAN_REASONS          \
  X(Unknown)                 \
  X(ServerIsClosed)          \
  X(SomeoneElseTriedToLogin) \
  X(YouAreAlreadyLoggedIn)

#define GAME_SERVER_STATES \
  X(Normal)                \
  X(AdultsOnly)            \
  X(InMaintenance)         \
  X(PayToPlay)             \
  X(Premium)

#define GAME_SERVER_BADGES \
  X(None)                  \
  X(New)

#define X(v) v,
DECLARE_ENUM(ClientState, std::uint8_t, CLIENT_STATES)
DECLARE_ENUM(GameState, std::uint8_t, GAME_STATES)
DECLARE_ENUM(ClientMode, std::uint8_t, CLIENT_MODES)
DECLARE_ENUM(CharacterGender, std::uint8_t, CHARACTER_GENDERS)
DECLARE_ENUM(LoginFailReason, std::uint8_t, LOGIN_FAIL_REASONS)
DECLARE_ENUM(BanReason, std::uint8_t, BAN_REASONS)
DECLARE_ENUM(GameServerState, std::uint8_t, GAME_SERVER_STATES)
DECLARE_ENUM(GameServerBadge, std::uint8_t, GAME_SERVER_BADGES)
#undef X

}  // namespace crayon