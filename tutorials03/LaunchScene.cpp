#include "LaunchScene.h"
#include "UILabel.h"
#include "UIButton.h"


bool LaunchScene::init()
{
    m_lb = std::shared_ptr<UILabel>(new UILabel("Hello World!"));
    m_lb->SetPosition({ 100,200 });
    m_lb->SetSize({ 300, 30 });
    m_lb->SetBackgroundColor(SDL_Color{255,0,0,255});
    AddNode(m_lb);

    m_btn = std::shared_ptr<UIButton>(new UIButton);
    m_btn->SetPosition({ 100, 260 });
    m_btn->SetText("Click me");
    m_btn->AddClick([=](void*) 
        {
            static int times = 0;
            char msg[64] = {};
            sprintf_s(msg, "hi you clicked button %d times", ++times);
            m_lb->SetText(msg);
        });
    AddNode(m_btn);

    m_btn2 = std::shared_ptr<UIButton>(new UIButton);
    m_btn2->SetPosition({ 100, 290 });
    m_btn2->SetSize({ 200,30 });
    m_btn2->SetText("disable button click me");
    m_btn2->AddClick([=](void*)
        {
                m_btn->SetEnabled(!m_btn->IsEnabled());
                if (m_btn->IsEnabled())
                {
                    m_btn2->SetText("disable button click me");
                }
                else
                {
                    m_btn2->SetText("enable button click me");
                }
        });
    AddNode(m_btn2);

    SDL_Texture* _text[4];
    _text[0] = sRender->LoadTextureFromFile("C:\\Users\\pan\\Pictures\\button\\normal.png");
    _text[1] = sRender->LoadTextureFromFile("C:\\Users\\pan\\Pictures\\button\\hover.png");
    _text[2] = sRender->LoadTextureFromFile("C:\\Users\\pan\\Pictures\\button\\press.png");
    _text[3] = sRender->LoadTextureFromFile("C:\\Users\\pan\\Pictures\\button\\disable.png");

    m_btn3 = std::shared_ptr<UIButton>(new UIButton(_text));
    m_btn3->SetPosition({ 100, 320 });
    m_btn3->SetText("Texture Button");
    m_btn3->AddClick([=](void*)
        {
            m_lb->SetText("Image Button Clicked");
        });
    AddNode(m_btn3);

    return true;
}

void LaunchScene::Update(uint32_t dt)
{

}
