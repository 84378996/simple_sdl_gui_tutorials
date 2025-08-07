#include "LaunchScene.h"
#include "UILabel.h"


bool LaunchScene::init()
{
    m_lb = std::shared_ptr<UILabel>(new UILabel("Hello World!"));
    m_lb->SetPosition({ 100,200 });
    m_lb->SetSize({ 100, 50 });
    m_lb->SetBackgroundColor(SDL_Color{255,0,0,255});
    AddNode(m_lb);
    return true;
}

void LaunchScene::Update(uint32_t dt)
{

}
