#ifndef PROTOCONVERTER_HH
#define PROTOCONVERTER_HH

#include "domain/gameevent.hh"
#include "types/color.hh"
#include "types/refstates.hh"

#include "3rdparty/proto/pb/ssl_gc_referee_message.pb.h"
#include "3rdparty/proto/pb/ssl_gc_game_event.pb.h"

namespace ProtoConverter {

RefereeStates::State convertRefCommand(ssl::Referee::Command cmd,
                                       Colors::Color ourColor = Colors::BLUE);

::GameEvent convertGameEvent(const ssl::GameEvent &protoEv);

} // namespace ProtoConverter

#endif