#include "LaunchScene.h"
#include "UILabel.h"
#include "UIButton.h"
#include "UITextEdit.h"
#include "UICheckBox.h"


bool LaunchScene::init()
{
    auto lb = std::shared_ptr<UILabel>(new UILabel("NickName"));
    lb->SetPosition({ 100,170 });
    lb->SetSize({ 80, 25 });
    AddNode(lb);

    auto tbnick = std::shared_ptr<UITextEdit>(new UITextEdit);
    tbnick->SetPosition({ 190, 170 });
    tbnick->SetSize({ 120,25 });
    AddNode(tbnick);

    m_lb = std::shared_ptr<UILabel>(new UILabel("UserName"));
    m_lb->SetPosition({ 100,200 });
    m_lb->SetSize({ 80, 25 });
    AddNode(m_lb);

    auto tb = std::shared_ptr<UITextEdit>(new UITextEdit);
    tb->SetPosition({ 190, 200 });
    tb->SetSize({120,25});
    AddNode(tb);

    lb = std::shared_ptr<UILabel>(new UILabel("Password"));
    lb->SetPosition({ 100,230 });
    lb->SetSize({ 80, 25 });
    AddNode(lb);

    auto pwd = std::shared_ptr<UITextEdit>(new UITextEdit);
    pwd->SetPosition({ 190, 230 });
    pwd->SetSize({ 120,25 });
    pwd->SetPassowrd(true);
    AddNode(pwd);

    m_btn = std::shared_ptr<UIButton>(new UIButton);
    m_btn->SetPosition({ 230, 270 });
    m_btn->SetText("Login");
    m_btn->AddClick([=](void*) 
        {
            auto nickname = tbnick->GetText();
            char szmsg[64];
            sprintf_s(szmsg, "Nickname is: %s", nickname.c_str());
            m_lbmsg->SetText(szmsg);
        });
    AddNode(m_btn);

    m_lbmsg = std::shared_ptr<UILabel>(new UILabel());
    m_lbmsg->SetPosition({ 100,310 });
    m_lbmsg->SetSize({ 350, 25 });
    AddNode(m_lbmsg);

    auto chk = std::shared_ptr<UICheckBox>(new UICheckBox("auto login"));
    chk->SetPosition({ 320,270 });
    chk->AddEvent(EventType::ET_CHECKCHANGED, [=](void*)
        {
            char szmsg[64];
            sprintf_s(szmsg, "auto login is: %s", chk->IsChecked() ? "checked" : "unchecked");
            m_lbmsg->SetText(szmsg);
        });
    AddNode(chk);
    return true;
}

void LaunchScene::Update(uint32_t dt)
{
    SceneBase::Update(dt);
}
