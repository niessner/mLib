
namespace ml
{

class EventMap
{
public:
    void registerEvent(const std::string &event, const std::function<void(std::vector<std::string> &params)> &handler);
    void dispatchEvents(ml::UIConnection &ui) const;
    void dispatchEvents(const std::vector<std::string> &messages) const;

private:
    std::map< std::string, std::function<void(std::vector<std::string> &params)> > _handlers;
};

}