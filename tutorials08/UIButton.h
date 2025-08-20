#include "UIControl.h"

class UIButton : public UIControl
{
public:
	UIButton(SDL_Texture** _text);
	UIButton();
	~UIButton() = default;

	void Render() override;
	//bool Process(SDL_Event* e) override;
	void SetResource(SDL_Texture** _tex);

private:
	SDL_Texture* m_tex[UIState::UIStateLength]{};
	static SDL_Color m_default_backgroud_color[UIState::UIStateLength];
};