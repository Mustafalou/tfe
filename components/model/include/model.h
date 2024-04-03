#include "open62541.h"
#include "accelstepper_wrapper.h"


extern int32_t nouv_position;

extern volatile bool motor_stop;
extern volatile bool motor_stop_RAZ;
void updateTargetPosition(int32_t new_position);


/*CONTROL DE POSITION*/
UA_StatusCode
setStepperPosition(UA_Server *server,
                   const UA_NodeId *sessionId, void *sessionContext,
                   const UA_NodeId *nodeId, void *nodeContext,
                   const UA_NumericRange *range, const UA_DataValue *data);
UA_StatusCode
readStepperPosition(UA_Server *server,
                    const UA_NodeId *sessionId, void *sessionContext,
                    const UA_NodeId *nodeId, void *nodeContext,
                    UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                    UA_DataValue *dataValue);

void 
addStepperControlNode(UA_Server *server, AccelStepperWrapper *stepper);

/*GESTION DE LA VITESSE MAX*/
UA_StatusCode
setStepperMaxSpeed(UA_Server *server,
                   const UA_NodeId *sessionId, void *sessionContext,
                   const UA_NodeId *nodeId, void *nodeContext,
                   const UA_NumericRange *range, const UA_DataValue *data);
UA_StatusCode
readStepperMaxSpeed(UA_Server *server,
                    const UA_NodeId *sessionId, void *sessionContext,
                    const UA_NodeId *nodeId, void *nodeContext,
                    UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                    UA_DataValue *dataValue);

void
addStepperMaxSpeedControlNode(UA_Server *server, AccelStepperWrapper *stepper);
/*GESTION DE l'ACCELERATION*/

UA_StatusCode
setStepperACC(UA_Server *server,
                   const UA_NodeId *sessionId, void *sessionContext,
                   const UA_NodeId *nodeId, void *nodeContext,
                   const UA_NumericRange *range, const UA_DataValue *data);
UA_StatusCode
readStepperACC(UA_Server *server,
                    const UA_NodeId *sessionId, void *sessionContext,
                    const UA_NodeId *nodeId, void *nodeContext,
                    UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                    UA_DataValue *dataValue);

void
addStepperAccControlNode(UA_Server *server, AccelStepperWrapper *stepper);

/*LECTURE VITESSE ACTUELLE*/

UA_StatusCode
readCurrentSpeed(UA_Server *server,
                    const UA_NodeId *sessionId, void *sessionContext,
                    const UA_NodeId *nodeId, void *nodeContext,
                    UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                    UA_DataValue *dataValue);
void
 addCurrentSpeed(UA_Server *server, AccelStepperWrapper *stepper);

/* STOP */
UA_StatusCode
readSTOPState(UA_Server *server,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_NodeId *nodeId, void *nodeContext,
                UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                UA_DataValue *dataValue);

UA_StatusCode
setSTOP(UA_Server *server,
                  const UA_NodeId *sessionId, void *sessionContext,
                  const UA_NodeId *nodeId, void *nodeContext,
                 const UA_NumericRange *range, const UA_DataValue *data);

void
addSTOPControlNode(UA_Server *server);
/*RAZ*/

UA_StatusCode
readRAZState(UA_Server *server,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_NodeId *nodeId, void *nodeContext,
                UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                UA_DataValue *dataValue);

UA_StatusCode
setRAZ(UA_Server *server,
                  const UA_NodeId *sessionId, void *sessionContext,
                  const UA_NodeId *nodeId, void *nodeContext,
                 const UA_NumericRange *range, const UA_DataValue *data);

void
addRAZControlNode(UA_Server *server);