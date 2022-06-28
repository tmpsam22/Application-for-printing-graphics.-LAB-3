/// @file ioc_container.h  представлена реализация IOC-контейнера
#pragma once

#ifndef IOC_CONTAINER_H
#define IOC_CONTAINER_H

#include <functional>
#include <cstdlib>
#include <memory>
#include <map>
#include <QtDebug>

/// @brief IOC-Container
/// Реализован в виде Синглтон
class IOCContainer
{

public:

    struct FactoryBase
    {
        virtual ~FactoryBase() = default;
    };

    template<typename T>
    class CFactory : public FactoryBase
    {

    public:

        ~CFactory() override
          {

          }

        CFactory(std::function<std::shared_ptr<T>()> functor)
            : functor(functor)
        {
             #ifdef DEBUG

                qDebug() << "ctor was triggered"  ;
             #endif
        }

        std::shared_ptr<T> GetObject()
        {
            return functor();
        }

     private:

        std::function<std::shared_ptr<T>()> functor;

    };


    /*
    Регистрация экземпляров
    */

    //Базовая реализация - регистрация функтора

    template<typename TInterface, typename ...TS>
    void RegisterFunctor(std::function<std::shared_ptr<TInterface>(std::shared_ptr<TS> ...ts)> functor)
    {
         #ifdef DEBUG

          qDebug() << "~~"  ;
          #endif
        factories[GetTypeID<TInterface>()] = std::make_shared<CFactory<TInterface>>([=] {return functor(GetObject<TS>()...); });

    }

    //Работаем с указателем на функцию
    template<typename TInterface, typename ...TS>
    void RegisterFunctor(std::shared_ptr<TInterface>(*functor)(std::shared_ptr<TS> ...ts))
    {
         #ifdef DEBUG

          qDebug() << "~~"  ;
          #endif
        RegisterFunctor(std::function<std::shared_ptr<TInterface>(std::shared_ptr<TS> ...ts)>(functor));
    }


    //Фабрика, которая будет вызывать необходимый конструктор для каждого экземпляра
    template<typename TInterface, typename TConcrete, typename ...TArguments>
    void RegisterFactory()
    {
         #ifdef DEBUG

          qDebug() << "~~"  ;
          #endif

        RegisterFunctor(std::function<std::shared_ptr<TInterface>(std::shared_ptr<TArguments> ...ts)>
          ([](std::shared_ptr<TArguments>...arguments) -> std::shared_ptr<TInterface>
          {
               return std::make_shared<TConcrete>(std::forward<std::shared_ptr<TArguments>>(arguments)...);
          })
        );

    }

    template<typename T>
    static int GetTypeID()
    {

        static int typeId = s_typeId++;
        #ifdef DEBUG

          qDebug() << "TypeId =\t" << typeId  ;
          qDebug() << "~~"  ;
        #endif
        return typeId;
    }

    template<typename T>
    std::shared_ptr<T> GetObject()
    {
        int typeId = GetTypeID<T>();
        #ifdef DEBUG

          qDebug() << "TypeId =\t" << typeId  ;
          qDebug() << "~~"  ;
        #endif

        auto factoryBase = factories[typeId];
        auto factory = std::static_pointer_cast<CFactory<T>>(factoryBase);

        #ifdef DEBUG
          qDebug() << "casted to CFACTORY from Factory Base"  ;
        #endif
        return factory->GetObject();
    }


    //Регистрация экземпляра объекта
    template<typename TInterface>
    void RegisterInstance(std::shared_ptr<TInterface> t)
    {
        #ifdef DEBUG

          qDebug() << "~~"  ;
          #endif
        factories[ GetTypeID<TInterface>() ] = std::make_shared<CFactory<TInterface>>([=] {return t; });
    }

    //Фабрика, которая будет возвращать один экземпляр  объекта  для каждого вызова
    template<typename TInterface, typename TConcrete, typename ...TArguments>
    void RegisterInstance()
    {
          #ifdef DEBUG

          qDebug() << "~~"  ;
          #endif
        // iface, derived classes
        // register
        RegisterInstance<TInterface>(std::make_shared<TConcrete>(GetObject<TArguments>()...));
    }

public:

    /// @brief получить IOC-контейнер
    static IOCContainer& IOCContainerInstance();

private:

    IOCContainer() = default;
    ~IOCContainer() = default;

private:

    IOCContainer(const IOCContainer&) = delete;
    IOCContainer(IOCContainer&&) = delete;
    IOCContainer& operator=(const IOCContainer&) = delete;
    IOCContainer& operator=(IOCContainer&&) = delete;

private:

     static int s_typeId;

     std::map<int, std::shared_ptr<FactoryBase>> factories;
};

#endif
