#include "ioc_container.h"

// определение s_typeId Ioc-контейнера
int IOCContainer::s_typeId = 17;

IOCContainer& IOCContainer::IOCContainerInstance()
{
    static IOCContainer ioc_;
    return ioc_;
}
