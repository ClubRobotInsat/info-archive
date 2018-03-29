//
// Created by terae on 27/01/18.
//

#ifndef ROOT_ENVIRONMENT_H
#define ROOT_ENVIRONMENT_H

#include <list>
#include <memory>

#include "Element.h"

namespace StrategyGenerator {
    class Element;

    using Table = std::list<std::shared_ptr<Element>>;
}

#endif // ROOT_ENVIRONMENT_H
