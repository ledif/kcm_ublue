#pragma once

#include <QString>

// Representation of the systemd unit ublue-rebase@.service
class RebaseService
{
public:
  static void startRebase(const QString&);
};