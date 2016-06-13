// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "AnimationsSample.h"

using namespace std;
using namespace ouzel;

AnimationsSample::AnimationsSample()
{
    scene::LayerPtr layer = make_shared<scene::Layer>();
    addLayer(layer);
    layer->setCamera(make_shared<scene::Camera>());

    scene::DebugDrawablePtr debugDrawable = make_shared<scene::DebugDrawable>();
    debugDrawable->rectangle(Rectangle(100.0f, 100.0f), graphics::Color(0, 128, 128, 255), true);
    debugDrawable->rectangle(Rectangle(100.0f, 100.0f), graphics::Color(255, 255, 255, 255), false);
    debugDrawable->line(Vector2(0.0f, 0.0f), Vector2(50.0f, 50.0f), graphics::Color(0, 255, 255, 255));
    debugDrawable->point(Vector2(75.0f, 75.0f), graphics::Color(255, 0, 0, 255));

    debugDrawable->circle(Vector2(75.0f, 75.0f), 20.0f, graphics::Color(0, 0, 255, 255));
    debugDrawable->circle(Vector2(25.0f, 75.0f), 20.0f, graphics::Color(0, 0, 255, 255), true);

    scene::NodePtr drawNode = make_shared<scene::Node>();
    drawNode->addDrawable(debugDrawable);
    drawNode->setPosition(Vector2(-300, 0.0f));
    layer->addChild(drawNode);

    drawNode->animate(make_shared<scene::Shake>(10.0f, Vector2(10.0f, 20.0f), 20.0f));

    scene::SpritePtr witchSprite = scene::Sprite::createFromFile("witch.png");

    ouzel::scene::NodePtr witch = make_shared<scene::Node>();
    witch->setPosition(Vector2(200, 0.0f));
    witch->addDrawable(witchSprite);
    layer->addChild(witch);

    vector<scene::AnimatorPtr> sequence = {
        make_shared<scene::Repeat>(make_shared<scene::Rotate>(1.0f, TAU, false), 3),
        make_shared<scene::Fade>(2.0f, 0.4f)
    };

    witch->animate(make_shared<scene::Sequence>(sequence));

    sharedEngine->getLocalization()->addLanguage("latvian", "lv.mo");
    sharedEngine->getLocalization()->setLanguage("latvian");

    gui::LabelPtr label = gui::Label::create("font.fnt", sharedEngine->getLocalization()->getString("Test"));
    layer->addChild(label);

    vector<scene::AnimatorPtr> sequence2 = {
        make_shared<scene::Animator>(1.0f), // delay
        make_shared<scene::Ease>(make_shared<scene::Move>(2.0f, Vector2(0.0f, -240.0f), false), scene::Ease::Type::OUT, scene::Ease::Func::BOUNCE)
    };

    label->animate(make_shared<scene::Sequence>(sequence2));
}

AnimationsSample::~AnimationsSample()
{
}
