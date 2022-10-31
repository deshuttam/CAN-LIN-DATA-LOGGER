# CAN-LIN-DATA-LOGGER
By Uttam Deshpande et. al.,



“CAN/LIN data logger” provides a simple and efficient means of logging the messages present on the CAN/LIN bus, process them, and display the same. The objective of logging the data present on the CAN/LIN bus is accomplished through the use of STM32F407VG microcontroller and MCP2551 CAN transceiver (for CAN bus only). The CAN/LIN data logger along with GUI could also be used to send messages periodically onto the CAN/LIN bus for fault detection and correction. 
* For more details refer to the work  [Self-health Monitoring and Reporting System for COVID-19 Patients Using CAN Data Logger](https://doi.org/10.30564/jcsr.v3i3.3487)

## Introduction
The modern automobiles incorporate about 70-150 Electronic Control Units (ECUs). These ECUs communicate the sensor data to each other over a robust vehicle bus standard that is the Controller Area Network (CAN) or its low-cost alternative that is Local Interconnection Network (LIN). The CAN or LIN messages can be read, processed, and analyzed to determine the functional parameters and monitor vehicle health. In case of a fault, CAN messages serve as a medium for fault identification and rectification. On the other hand, LIN messages are used in not-so-crucial applications. These advancements call for the development of efficient systems called “Data Loggers” which can record and examine the messages on the CAN/LIN bus.
OBD (On Board Diagnostics) is a self diagnostics and reporting capability that an example mechanics use to identify what is wrong in the car. The OBD standard specifies the number of standardized Diagnostic Trouble Code (DTC) which can be retrieved via CAN/LIN logger or diagnostics tool for any vehicle. OBD is also used for an example real-time vehicle telematics or post analysis using CAN/LIN data loggers, applications include vehicle fleet management, fuel optimization proactive vehicle diagnostics and more.
![image](https://user-images.githubusercontent.com/107185323/198985188-d53ef5e4-bedf-4221-aa04-18d0712a6caf.png)

![image](https://user-images.githubusercontent.com/107185323/198985461-5d1fc901-0883-4536-a480-77638876f4ba.png)

![image](https://user-images.githubusercontent.com/107185323/198985516-8f0da875-9950-407b-8666-41d07d9e4db2.png)


The repository includes:
* Microvsion Keil Source code for CAN LIN Data Logger.
* Visual Studio, C code.

## Citing
@ARTICLE{ 10.30564/jcsr.v3i3.3487., AUTHOR={Deshpande, Uttam U. et al.,}, TITLE={Self-health Monitoring and Reporting System for COVID-19 Patients Using CAN Data Logger}, JOURNAL={Journal of Computer Science Research}, VOLUME={3}, YEAR={2021}, URL={ https://ojs.bilpublishing.com/index.php/jcsr/article/view/3487 }, DOI={ https://doi.org/10.30564/jcsr.v3i3.3487 }, ISSN={2630-5151}, }

Requirements: Software
* Microvsion Keil
* Visual Studio
* Arduino 1.8.9

Requirements: Hardware
* STM32F407VGTx Microcontroller
* MCP2551 CAN transceiver
* FTDI module
* Arduino UNO (ATmega328P)
* MCP2515 CAN shield

## Test Cases
Case 1: Periodic transmission of CAN data frames as per the time interval specified on GUI.
Inputs: Five CAN data frames.
Expected Output: Successfully transmitted CAN data frames must be displayed on GUI along with the time elapsed and STATUS checkbox be ticked.

![image](https://user-images.githubusercontent.com/107185323/198985851-8078d090-b2ba-4aae-8a85-23998f7e2338.png)

Case 2: Periodic request of CAN remote frames as per the time interval specified on GUI.
Inputs: Five CAN remote frames.
Expected Output: Successfully received CAN data frames must be displayed on GUI along with the time elapsed and STATUS checkbox be ticked.

![image](https://user-images.githubusercontent.com/107185323/198985997-8468a7cb-18d2-4af6-98ee-e099ec1d5f74.png)

Case 3: Simultaneous transmission or request of LIN frames as per the time interval specified on GUI.
Inputs: CAN data frames and CAN remote frames.
Expected Output: Successfully transmitted LIN messages or received request messages must be displayed on GUI along with the time elapsed and STATUS checkbox 
be ticked.
![image](https://user-images.githubusercontent.com/107185323/198986130-68305ba5-f6be-48d0-b6b2-70776ca5c798.png)

Case 4: Reception of LIN messages.
Expected Output: Received valid CAN data frames must be displayed on GUI along with the time elapsed.
![image](https://user-images.githubusercontent.com/107185323/198986639-777796de-b499-4436-82e3-8ddb6b3954ed.png)


## Usage
A simple data logging using device using CAN/LIN bus can be implemented using this method. The presence of both CAN and LIN data loggers in a single system makes in an efficient system. The GUI designed to provide an interface for the user to communicate with the CAN/LIN data logger adds to the usability of the system. The system can be used to send CAN/LIN messages on the CAN/LIN bus respectively, to verify the functional behaviour of nodes on CAN bus. The data present on CAN/LIN bus can be logged for identifying and rectifying faults in communication by live streaming the data or by saving the data for later reference. The logger can also request data from other nodes to verify the message being communicated among the nodes. All these features add to the functionality of the system. The messages that are transmitted or received are in human readable format which makes the system user-friendly. The CAN/LIN data loggers can be used by system designers and manufactures to test and validate their product before releasing into the market, and also by service providers for diagnosing faults. The logger finds application not only in the automotive field, but also in the field of agriculture, home automation, aviation, navigation, medicine etc. In general, they can be employed in any fields where the baseline of communication among the nodes is CAN and LIN protocol
