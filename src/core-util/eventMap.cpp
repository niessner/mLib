
namespace ml
{

void EventMap::registerEvent(const string &event, const std::function<void(vector<string> &params)> &handler)
{
    _handlers[event] = handler;
}

void EventMap::dispatchEvents(ml::UIConnection &ui) const
{
    ui.readMessages();
    dispatchEvents(ui.messages());
    ui.messages().clear();
}

void EventMap::dispatchEvents(const vector<string> &messages) const
{
    for (const string &message : messages)
    {
        auto parts = ml::util::split(message, ' ');
        if (_handlers.count(parts[0]) == 0)
        {
            cout << "No event handler for: " << message << endl;
        }
        else
        {
            _handlers.find(parts[0])->second(parts);
        }
    }
}

}