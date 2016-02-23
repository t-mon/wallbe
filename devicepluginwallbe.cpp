/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Copyright (C) 2015 Simon Stuerz <simon.stuerz@guh.guru>                *
 *                                                                         *
 *  This file is part of guh.                                              *
 *                                                                         *
 *  Guh is free software: you can redistribute it and/or modify            *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, version 2 of the License.                *
 *                                                                         *
 *  Guh is distributed in the hope that it will be useful,                 *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with guh. If not, see <http://www.gnu.org/licenses/>.            *
 *                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "devicepluginwallbe.h"
#include "plugininfo.h"

// Note: You can find the documentation for this code here -> http://dev.guh.guru/write-plugins.html

/* The constructor of this device plugin. */
DevicePluginWallbe::DevicePluginWallbe()
{

}

/* This method will be called from the devicemanager to get
 * information about this plugin which device resource will be needed.
 *
 * For multiple resources use the OR operator:
 * Example:
 *
 * return DeviceManager::HardwareResourceTimer | DeviceManager::HardwareResourceNetworkManager;
 *
 */
DeviceManager::HardwareResources DevicePluginWallbe::requiredHardware() const
{
    return DeviceManager::HardwareResourceTimer |
            DeviceManager::HardwareResourceNetworkManager;
}

/* This method will be called from the devicemanager while he
 * is setting up a new device. Here the developer has the chance to
 * perform the setup on the actual device and report the result.
 */
DeviceManager::DeviceSetupStatus DevicePluginWallbe::setupDevice(Device *device)
{
    Q_UNUSED(device)
    qCDebug(dcWallbe) << "Hello world! Setting up a new device:" << device->name();
    qCDebug(dcWallbe) << "The new device has the DeviceId" << device->id().toString();
    qCDebug(dcWallbe) << device->params();



    mb = modbus_new_tcp("192.168.0.8", 502);
    modbus_connect(mb);

    return DeviceManager::DeviceSetupStatusSuccess;
}


DeviceManager::DeviceError DevicePluginWallbe::executeAction(Device *device, const Action &action)
{
    if (device->deviceClassId() == wallbeDeviceClassId ) {

        // check if this is the "set power" action
        if (action.actionTypeId() == chargingActionTypeId) {

            // get the param value
            Param chargingParam = action.param("power");
            bool power = chargingParam.value().toBool();

            qCDebug(dcWallbe) << "start Charging button" << device->paramValue("name").toString() << "set power to" << power;

            // Set the "power" state
            device->setStateValue(chargingStateTypeId, power);

            return DeviceManager::DeviceErrorNoError;
        }
        return DeviceManager::DeviceErrorActionTypeNotFound;
    }
    return DeviceManager::DeviceErrorDeviceClassNotFound;
}



void DevicePluginWallbe::deviceRemoved(Device *device)
{
   modbus_close(mb);
   modbus_free(mb);
   qCDebug(dcWallbe) << "Remove device" << device->paramValue("name").toString();

}
