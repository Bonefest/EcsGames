#ifndef ENTITYSTATUS_H_INCLUDED
#define ENTITYSTATUS_H_INCLUDED

#include "common.h"

struct StatusInfo {
	uint32_t dialogID;
	Text greetingText;
};

class EntityRelationStatus {
public:
	void setStatusDialog(uint32_t status, StatusInfo info) {
		_statusInfo[status] = info;
	}

	void setCurrentStatus(uint32_t status) {
		_currentStatus = status;
	}

	StatusInfo getCurrentStatusInfo() {
		return _statusInfo[_currentStatus];
	}
private:
	map<uint32_t, StatusInfo> _statusInfo;
	uint32_t _currentStatus;
};

#endif // ENTITYSTATUS_H_INCLUDED
