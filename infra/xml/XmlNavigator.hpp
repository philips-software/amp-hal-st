#ifndef XML_OBJECT_NAVIGATOR_HPP
#define XML_OBJECT_NAVIGATOR_HPP

#include "pugixml.hpp"
#include "infra/util/Optional.hpp"
#include <functional>

namespace infra
{
    class XmlNodeNavigator;
    //class XmlOptionalObjectNavigator;

    struct XmlNodeNavigatorToken
    {
        std::string name;
    };

    struct XmlStringAttributeNavigatorToken
    {
        std::string name;
    };

    struct XmlOptionalStringAttributeNavigatorToken
    {
        std::string name;
    };

    struct XmlIntegerAttributeNavigatorToken
    {
        std::string name;
    };

    struct XmlOptionalIntegerAttributeNavigatorToken
    {
        std::string name;
    };

    template<class Result>
    struct XmlTransformObjectNavigatorToken
    {
        std::string name;
        std::function<Result(const XmlNodeNavigator& navigator)> transformation;
    };

    template<class Result>
    struct XmlTransformArrayNavigatorToken
    {
        std::string name;
        std::function<Result(const XmlNodeNavigator& navigator)> transformation;
    };

    //template<class Result>
    //struct XmlTransformOptionalObjectNavigatorToken
    //{
    //    std::string name;
    //    std::function<Result(const XmlNodeNavigator& navigator)> transformation;
    //};

    class XmlNodeNavigator
    {
    public:
        explicit XmlNodeNavigator(const std::string& contents);
        XmlNodeNavigator(const pugi::xml_node& node);

        XmlNodeNavigator operator/(XmlNodeNavigatorToken token) const;
        std::string operator/(XmlStringAttributeNavigatorToken token) const;
        infra::Optional<std::string> operator/(XmlOptionalStringAttributeNavigatorToken token) const;
        int32_t operator/(XmlIntegerAttributeNavigatorToken token) const;
        infra::Optional<int32_t> operator/(XmlOptionalIntegerAttributeNavigatorToken token) const;

        template<class Result>
        Result operator/(XmlTransformObjectNavigatorToken<Result> token) const;
        //    template<class Result>
        //    infra::Optional<Result> operator/(XmlTransformOptionalObjectNavigatorToken<Result> token) const;
        template<class Result>
        std::vector<Result> operator/(XmlTransformArrayNavigatorToken<Result> token) const;

    protected:
        pugi::xml_document document;
        pugi::xml_node node;
    };

    //class XmlOptionalObjectNavigator
    //{
    //public:
    //    XmlOptionalObjectNavigator() = default;
    //    XmlOptionalObjectNavigator(infra::XmlObject& object);
    //    XmlOptionalObjectNavigator(const XmlNodeNavigator& navigator);

    //    XmlOptionalObjectNavigator operator/(XmlNodeNavigatorToken token) const;
    //    infra::Optional<std::string> operator/(XmlStringAttributeNavigatorToken token) const;
    //    infra::Optional<std::string> operator/(XmlOptionalStringAttributeNavigatorToken token) const;
    //    infra::Optional<int32_t> operator/(XmlIntegerAttributeNavigatorToken token) const;

    //    template<class Result>
    //    infra::Optional<Result> operator/(XmlTransformObjectNavigatorToken<Result> token) const;
    //    template<class Result>
    //    infra::Optional<Result> operator/(XmlTransformOptionalObjectNavigatorToken<Result> token) const;

    //protected:
    //    infra::Optional<XmlNodeNavigator> navigator;
    //};


    //// Implementation    ////

    template<class Result>
    Result XmlNodeNavigator::operator/(XmlTransformObjectNavigatorToken<Result> token) const
    {
        auto child = node.child(token.name.c_str());
        if (child == pugi::xml_node())
            throw std::runtime_error(("Child " + token.name + " not found").c_str());

        return token.transformation(child);
    }

    //template<class Result>
    //infra::Optional<Result> XmlNodeNavigator::operator/(XmlTransformOptionalObjectNavigatorToken<Result> token) const
    //{
    //    auto subObject = object.GetOptionalObject(token.name);
    //    if (subObject == infra::none)
    //        return {};

    //    return infra::MakeOptional(token.transformation(*subObject));
    //}

    template<class Result>
    std::vector<Result> XmlNodeNavigator::operator/(XmlTransformArrayNavigatorToken<Result> token) const
    {
        std::vector<Result> result;

        for (auto child: node.children(token.name.c_str()))
            result.push_back(token.transformation(child));

        return result;
    }

    // template<class Result>
    //infra::Optional<Result> XmlOptionalObjectNavigator::operator/(XmlTransformObjectNavigatorToken<Result> token) const
    //{
    //    if (navigator != infra::none)
    //        return infra::MakeOptional(*navigator / token);
    //    else
    //        return {};
    //}

    //template<class Result>
    //infra::Optional<Result> XmlOptionalObjectNavigator::operator/(XmlTransformOptionalObjectNavigatorToken<Result> token) const
    //{
    //    if (navigator != infra::none)
    //        return *navigator / token;
    //    else
    //        return {};
    //}
}

#endif
