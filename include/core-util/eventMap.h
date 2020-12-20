#ifndef _EVENT_MAP_H_
#define _EVENT_MAP_H_

#include <map>
#include <core-util/UIConnection.h>

namespace ml
{

class EventMap
{
public:
    void registerEvent(const std::string &event, const std::function<void(std::vector<std::string> &params)> &handler);
    void dispatchEvents(const std::vector<std::string> &messages) const;
#ifdef _WIN32
	void dispatchEvents(ml::UIConnection &ui) const;
#endif
private:
    std::map< std::string, std::function<void(std::vector<std::string> &params)> > _handlers;
};

}
#endif