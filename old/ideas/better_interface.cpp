// old
mgr.forEntitiesMatching<SRenderRectangle>([this](auto, auto&, auto& cRender)
    {
        this->render(cRender.shape);
    });

// new
mgr.forEntitiesMatching<SRenderRectangle>().pick<CRender>([this](auto& cRender)
    {
        this->render(cRender.shape);
    });



// old
using MyComponents = ecs::ComponentList<CPosition, CVelocity, ...>;
using MySettings = ecs::Settings<MyComponents, MyTags, MySignatures>;
using MyManager = ecs::Manager<MySettings>;

// new
auto my_components =
    ecs::make_component_list(type_v<CPosition>, type_v<CVelocity>, ...);
auto my_settings = ecs::make_settings(my_components, my_tags, my_signatures);
auto my_manager = ecs::make_manager(my_settings);