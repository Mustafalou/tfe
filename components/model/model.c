#include "open62541.h"
#include "model.h"
#include "driver/gpio.h"
#include "accelstepper_wrapper.h"


/*DECLARATION VARIABLES*/
int32_t nouv_position = 0;
int32_t position=0;
int32_t vit_position =0;
UA_Float new_acc = 0.0;
UA_Float new_speed = 0.0;
volatile bool motor_stop_RAZ = false; 
volatile bool motor_stop = false;
void updateTargetPosition(int32_t new_position) {
    nouv_position = new_position;
    
}

/*CONTROLE POSITION*/
UA_StatusCode 
readStepperPosition(UA_Server *server,
                                  const UA_NodeId *sessionId, void *sessionContext,
                                  const UA_NodeId *nodeId, void *nodeContext,
                                  UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                                  UA_DataValue *dataValue) {
    AccelStepperWrapper *stepper = (AccelStepperWrapper *)nodeContext;
    

    if (stepper == NULL) {
        UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Stepper pointer is NULL");
        return UA_STATUSCODE_BADINTERNALERROR;
    }
    position = accelstepper_current_position(stepper);
    UA_Variant_setScalarCopy(&dataValue->value, &position, &UA_TYPES[UA_TYPES_INT32]);
    dataValue->hasValue = true; 
    return UA_STATUSCODE_GOOD;
} 

UA_StatusCode
setStepperPosition(UA_Server *server,
                   const UA_NodeId *sessionId, void *sessionContext,
                   const UA_NodeId *nodeId, void *nodeContext,
                   const UA_NumericRange *range, const UA_DataValue *data) {
    AccelStepperWrapper *stepper = (AccelStepperWrapper *)nodeContext;

    // Obtenez la nouvelle position à partir des données
    int32_t new_position = *(UA_Int32 *)data->value.data;

    updateTargetPosition(new_position);
    accelstepper_move_to(stepper, new_position);
    return UA_STATUSCODE_GOOD;
    


}

void addStepperControlNode(UA_Server *server, AccelStepperWrapper *stepper) {
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "stepmotor Position");
    attr.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "stepmotor Position");
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "stepmotor Position");
     UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    UA_DataSource stepperDataSource;
    stepperDataSource.read = readStepperPosition;
    stepperDataSource.write = setStepperPosition;

                                     
    UA_Server_addDataSourceVariableNode(server, currentNodeId,
                                                        parentNodeId,
                                        parentReferenceNodeId, currentName,
                                        variableTypeNodeId,attr, stepperDataSource, (void *)stepper, NULL);
                                                        
    }                                                    
/*CONTROLE VITESSE MAX*/
UA_StatusCode 
readStepperMaxSpeed(UA_Server *server,
                                  const UA_NodeId *sessionId, void *sessionContext,
                                  const UA_NodeId *nodeId, void *nodeContext,
                                  UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                                  UA_DataValue *dataValue) {
    AccelStepperWrapper *stepper = (AccelStepperWrapper *)nodeContext;
     // ou une autre valeur par défaut
    UA_Variant_setScalarCopy(&dataValue->value, &new_speed, &UA_TYPES[UA_TYPES_FLOAT]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;

}
UA_StatusCode 
setStepperMaxSpeed(UA_Server *server,
                               const UA_NodeId *sessionId, void *sessionContext,
                               const UA_NodeId *nodeId, void *nodeContext,
                               const UA_NumericRange *range, const UA_DataValue *data) {
    AccelStepperWrapper *stepper = (AccelStepperWrapper *)nodeContext;
    new_speed = *(UA_Float *)data->value.data;
    accelstepper_set_max_speed(stepper, new_speed);
    ESP_LOGI("Step Init", "VITESSE: %f", new_speed);
    return UA_STATUSCODE_GOOD;
}
void addStepperMaxSpeedControlNode(UA_Server *server, AccelStepperWrapper *stepper) {
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "stepmotor MaxSpeed");
    attr.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "stepmotor MaxSpeed");
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "stepmotor MaxSpeed");
     UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    UA_DataSource SpeedDataSource;
    SpeedDataSource.read = readStepperMaxSpeed;
    SpeedDataSource.write = setStepperMaxSpeed;

                                   
    UA_Server_addDataSourceVariableNode(server, currentNodeId,
                                                        parentNodeId,
                                        parentReferenceNodeId, currentName,
                                        variableTypeNodeId,attr, SpeedDataSource, (void *)stepper, NULL);
     
                                                        
    }                                                   
/*CONTROLE ACCELERATION*/

UA_StatusCode 
readStepperACC(UA_Server *server,
                                  const UA_NodeId *sessionId, void *sessionContext,
                                  const UA_NodeId *nodeId, void *nodeContext,
                                  UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                                  UA_DataValue *dataValue) {
    AccelStepperWrapper *stepper = (AccelStepperWrapper *)nodeContext;
    // ou une autre valeur par défaut
    UA_Variant_setScalarCopy(&dataValue->value, &new_acc, &UA_TYPES[UA_TYPES_FLOAT]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;

}
UA_StatusCode 
setStepperACC(UA_Server *server,
                               const UA_NodeId *sessionId, void *sessionContext,
                               const UA_NodeId *nodeId, void *nodeContext,
                               const UA_NumericRange *range, const UA_DataValue *data) {
    AccelStepperWrapper *stepper = (AccelStepperWrapper *)nodeContext;
    new_acc = *(UA_Float *)data->value.data;
    accelstepper_set_acceleration(stepper, new_acc);
    ESP_LOGI("Step Init", "ACC: %f", new_acc);
    return UA_STATUSCODE_GOOD;
}
void addStepperAccControlNode(UA_Server *server, AccelStepperWrapper *stepper) {
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "stepmotor Acc");
    attr.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "stepmotor Acc");
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "stepmotor Acc");
     UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    UA_DataSource ACCDataSource;
    ACCDataSource.read = readStepperACC;
    ACCDataSource.write = setStepperACC;

                                   
    UA_Server_addDataSourceVariableNode(server, currentNodeId,
                                                        parentNodeId,
                                        parentReferenceNodeId, currentName,
                                        variableTypeNodeId,attr, ACCDataSource, (void *)stepper, NULL);
     
                                                        
    }
    /*VITESSE ACTUELLE*/
    UA_StatusCode
    readCurrentSpeed(UA_Server*server,
                                  const UA_NodeId *sessionId, void *sessionContext,
                                  const UA_NodeId *nodeId, void *nodeContext,
                                  UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                                  UA_DataValue *dataValue) {
    AccelStepperWrapper *stepper = (AccelStepperWrapper *)nodeContext;
    
    vit_position = accelstepper_current_position(stepper);
    UA_Variant_setScalarCopy(&dataValue->value, &vit_position, &UA_TYPES[UA_TYPES_INT32]);
    dataValue->hasValue = true; 
    return UA_STATUSCODE_GOOD;
} 



void
addCurrentSpeed(UA_Server *server, AccelStepperWrapper *stepper) {
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "Vitesse actuelle");
    attr.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ;

    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "Vitesse actuelle");
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "Vitesse actuelle");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    UA_DataSource SpeedActDataSource;
    SpeedActDataSource.read = readCurrentSpeed;
    UA_Server_addDataSourceVariableNode(server, currentNodeId,
                                                        parentNodeId,
                                        parentReferenceNodeId, currentName,
                                        variableTypeNodeId,attr, SpeedActDataSource, (void *)stepper, NULL);
}
/*-----------------------------------------------*/
/*NOEUD ARRET*/ 

UA_StatusCode
readSTOPState(UA_Server *server,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_NodeId *nodeId, void *nodeContext,
                UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                UA_DataValue *dataValue) {
    
    UA_Variant_setScalarCopy(&dataValue->value, &motor_stop,
                             &UA_TYPES[UA_TYPES_BOOLEAN]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode
setSTOP(UA_Server *server,
                  const UA_NodeId *sessionId, void *sessionContext,
                  const UA_NodeId *nodeId, void *nodeContext,
                 const UA_NumericRange *range, const UA_DataValue *data) {
    motor_stop = *(UA_Boolean *)data->value.data;
    
    
    //UA_StatusCode status = relay1_state_after_write == level ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADINTERNALERROR;
    return UA_STATUSCODE_GOOD;
}

void
addSTOPControlNode(UA_Server *server) {
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "STOP");
    attr.dataType = UA_TYPES[UA_TYPES_BOOLEAN].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "STOP");
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "STOP");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    UA_DataSource STOP;
    STOP.read = readSTOPState;
    STOP.write = setSTOP;
    UA_Server_addDataSourceVariableNode(server, currentNodeId, parentNodeId,
                                        parentReferenceNodeId, currentName,
                                        variableTypeNodeId, attr,
                                        STOP, NULL, NULL);
}
/*NOEUD ARRET ET REMISE POSITION INITIALE */
UA_StatusCode
readRAZState(UA_Server *server,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_NodeId *nodeId, void *nodeContext,
                UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                UA_DataValue *dataValue) {
    
    UA_Variant_setScalarCopy(&dataValue->value, &motor_stop_RAZ,
                             &UA_TYPES[UA_TYPES_BOOLEAN]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode
setRAZState(UA_Server *server,
                  const UA_NodeId *sessionId, void *sessionContext,
                  const UA_NodeId *nodeId, void *nodeContext,
                 const UA_NumericRange *range, const UA_DataValue *data) {
    motor_stop_RAZ = *(UA_Boolean *)data->value.data;
    
    
    //UA_StatusCode status = relay1_state_after_write == level ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADINTERNALERROR;
    return UA_STATUSCODE_GOOD;
}

void
addRAZControlNode(UA_Server *server) {
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "STOP_RAZ");
    attr.dataType = UA_TYPES[UA_TYPES_BOOLEAN].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "STOP_RAZ");
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "STOP_RAZ");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    UA_DataSource RAZ;
    RAZ.read = readRAZState;
    RAZ.write = setRAZState;
    UA_Server_addDataSourceVariableNode(server, currentNodeId, parentNodeId,
                                        parentReferenceNodeId, currentName,
                                        variableTypeNodeId, attr,
                                        RAZ, NULL, NULL);
}