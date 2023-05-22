meta:
  id: server_packets
  file-extension: stream
  license: MIT
  endian: le
  encoding: UTF-8

seq:
  - id: packet_id
    type: u2
    enum: packet_id
  - id: payload
    type:
      switch-on: packet_id
      cases:
        'packet_id::char_list': char_list_payload
        'packet_id::char_server_rejected': char_server_rejected_payload
        'packet_id::login_details': login_details_payload
        'packet_id::login_failed': login_failed_payload
        'packet_id::ban_notification': ban_notification_payload
        'packet_id::char_rename_response': char_rename_response_payload
        'packet_id::char_select_window': char_select_window_payload
        'packet_id::char_display_page': char_display_page_payload
        'packet_id::pincode_state': pincode_state_payload
        _: unknown_payload
enums:
  packet_id:
    0x006B: char_list
    0x006C: char_server_rejected
    0x0AC4: login_details
    0x083E: login_failed
    0x0081: ban_notification
    0x082D: char_select_window
    0x08B9: pincode_state
    0x08FD: char_rename_response
    0x09A0: char_display_page
  gender:
    0: female
    1: male
  login_failed_reason:
    0: unregistered_user
    1: incorrect_password
    2: user_expired
    3: rejected_from_server
    4: blocked_by_game_master_team
    5: outdated_client
    6: login_prohibited_until
    7: server_overload
    8: no_more_accounts_maybe_connected_from_company
    9: banned_by_dba
    10: email_not_confirmed
    11: banned_by_game_master
    12: temporary_ban_for_db_work
    13: self_locked
    14: unpermitted_group_01
    15: unpermitted_group_02
    99: user_deleted
    100: login_information_remains_at
    101: account_locked_for_hacking_investigation
    102: temporary_ban_to_investigate_bug
    103: character_being_deleted_01
    104: character_being_deleted_02
    255: unknown
  ban_reason:
    1: server_closed
    2: someone_else_logged_in
    8: already_logged_in
  server_state:
    0: normal
    1: maintenance
    2: adult_only
    3: premium
    4: pay_to_play
  server_property:
    0: none
    1: new_server
  char_reject_reason:
    0: server_overloaded
  char_rename_result:
    0: successful
    1: name_already_changed
    2: incorrect_user_info
    3: failed_to_change_name
    4: name_already_used
    5: leave_guild
    6: leave_party
    7: length_exceeded
    8: contains_invalid_characters
    9: name_change_prohibited
    10: unknown_error
  pincode_request:
    0: ok
    1: ask
    2: not_set
    3: expired
    4: create
    5: illegal
    8: incorrect
types:
  unknown_payload:
    seq:
      - id: bytes
        type: u1
        repeat: expr
        repeat-expr: 0
  character_server:
    seq:
      - id: ip
        type: u4be
      - id: port
        type: u2
      - id: name
        type: str
        size: 20
      - id: active_users
        type: u2
      - id: state
        type: u2
        enum: server_state
      - id: property
        type: u2
        enum: server_property
      - id: unknown
        type: u1
        repeat: expr
        repeat-expr: 128
  login_details_payload:
    seq:
      - id: packet_size
        type: u2
      - id: login_id
        type: u4
      - id: account_id
        type: u4
      - id: session_id
        type: u4
      - id: last_login_ip
        type: u4
      - id: last_login_time
        type: str
        size: 24
      - id: unknown
        type: u2
      - id: gender
        type: u1
        enum: gender
      - id: web_auth_token
        type: u1
        repeat: expr
        repeat-expr: 17
      - id: servers
        type: character_server
        repeat: expr
        repeat-expr: (packet_size - 64) / sizeof<character_server>
  login_failed_payload:
    seq:
      - id: error_code
        type: u1
        enum: login_failed_reason
      - id: details
        type: str
        size: 20
  ban_notification_payload:
    seq:
      - id: reason
        type: u1
        enum: ban_reason
  char_rename_response_payload:
    seq:
      - id: result
        type: u4
        enum: char_rename_result
  char_select_window_payload:
    seq:
      - id: packet_size
        type: u2
      - id: normal_slots
        type: u1
      - id: premium_slots
        type: u1
      - id: billing_slots
        type: u1
      - id: character_slots
        type: u1
      - id: max_slots
        type: u1
      - id: unused_bytes
        type: u1
        repeat: expr
        repeat-expr: 20
  char_details:
    seq:
      - id: character_id
        type: u4
      - id: base_experience
        type: u8
      - id: money
        type: u4
      - id: job_experience
        type: u8
      - id: job_level
        type: u4
      - id: body_state
        type: u4
      - id: health_state
        type: u4
      - id: effect_state
        type: u4
      - id: karma
        type: u4
      - id: honor
        type: u4
      - id: status_points
        type: u2
      - id: health
        type: u8
      - id: max_health
        type: u8
      - id: mana
        type: u8
      - id: max_mana
        type: u8
      - id: walk_speed
        type: u2
      - id: job
        type: u2
      - id: hair_style
        type: u2
      - id: body
        type: u2
      - id: right_hand
        type: u2
      - id: base_level
        type: u2
      - id: job_skill_points
        type: u2
      - id: head_bottom
        type: u2
      - id: left_hand
        type: u2
      - id: head_top
        type: u2
      - id: head_mid
        type: u2
      - id: hair_palette
        type: u2
      - id: body_palette
        type: u2
      - id: name
        type: str
        size: 24
      - id: stat_str
        type: u1
      - id: stat_agi
        type: u1
      - id: stat_vit
        type: u1
      - id: stat_int
        type: u1
      - id: stat_dex
        type: u1
      - id: stat_luk
        type: u1
      - id: slot
        type: u1
      - id: hair_color
        type: u1
      - id: is_renamed
        type: u2
      - id: map_name
        type: str
        size: 16
      - id: delete_rev_date
        type: u4
      - id: robe
        type: u4
      - id: slot_change_count
        type: u4
      - id: name_change_count
        type: u4
      - id: gender
        type: u1
        enum: gender
  char_list_payload:
    seq:
      - id: packet_size
        type: u2
      - id: max_slots
        type: u1
      - id: available_slots
        type: u1
      - id: premium_slots
        type: u1
      - id: characters
        type: char_details
        repeat: expr
        repeat-expr: (packet_size - 7) / sizeof<char_details>
  char_display_page_payload:
    seq:
      - id: page_to_load
        type: u4
  char_server_rejected_payload:
    seq:
      - id: reason
        type: u1
        enum: char_reject_reason
  pincode_state_payload:
    seq:
      - id: seed
        type: u4
      - id: account_id
        type: u4
      - id: request
        type: u2
        enum: pincode_request