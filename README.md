# CAN-LIN-DATA-LOGGER

By Uttam Deshpande et. al.,
“CAN/LIN data logger” provides a simple and efficient means of logging the messages present on the CAN/LIN bus, process them, and display the same. The objective of logging the data present on the CAN/LIN bus is accomplished through the use of STM32F407VG microcontroller and MCP2551 CAN transceiver (for CAN bus only). The CAN/LIN data logger along with GUI could also be used to send messages periodically onto the CAN/LIN bus for fault detection and correction. 
* For more details refer to the work  [Self-health Monitoring and Reporting System for COVID-19 Patients Using CAN Data Logger](https://doi.org/10.30564/jcsr.v3i3.3487)

## Introduction
The modern automobiles incorporate about 70-150 Electronic Control Units (ECUs). These ECUs communicate the sensor data to each other over a robust vehicle bus standard that is the Controller Area Network (CAN) or its low-cost alternative that is Local Interconnection Network (LIN). The CAN or LIN messages can be read, processed, and analyzed to determine the functional parameters and monitor vehicle health. In case of a fault, CAN messages serve as a medium for fault identification and rectification. On the other hand, LIN messages are used in not-so-crucial applications. These advancements call for the development of efficient systems called “Data Loggers” which can record and examine the messages on the CAN/LIN bus.

