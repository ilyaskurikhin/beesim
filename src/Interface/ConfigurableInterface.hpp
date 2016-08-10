/*
 * Configurable.hpp
 *
 *  Created on: May 13, 2016
 *      Author: ilya
 */

#ifndef CONFIGURABLE_HPP_
#define CONFIGURABLE_HPP_

class ConfigurableInterface
{
public:

    virtual
    ~ConfigurableInterface() = default;

    virtual void
    reloadConfig() =0;

};



#endif /* PARTIE1_SRC_INTERFACE_CONFIGURABLE_HPP_ */
