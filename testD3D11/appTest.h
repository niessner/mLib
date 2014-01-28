
class AppTest : public ApplicationCallback
{
public:
	void init(ApplicationData &app);
	void render(ApplicationData &app);
	void keyPress(ApplicationData &app, UINT key);
};