// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2020 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END


#include "gfx/legato/widget/radiobutton/legato_radiobutton_group.h"

#if LE_RADIOBUTTON_WIDGET_ENABLED == 1

#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/widget/radiobutton/legato_widget_radiobutton.h"

leResult leRadioButtonGroup_Create(leRadioButtonGroup** grp)
{
    leRadioButtonGroup* btnGrp;
    
    if(grp == NULL)
        return LE_FAILURE;
        
    *grp = NULL;
    
    btnGrp = LE_MALLOC(sizeof(leRadioButtonGroup));

    if(btnGrp == NULL)
        return LE_FAILURE;

    btnGrp->selected = NULL;
    btnGrp->initialized = LE_FALSE;

    if(leArray_Create(&btnGrp->buttonList) == LE_FAILURE)
        return LE_FAILURE;

    btnGrp->initialized = LE_TRUE;
    
    *grp = btnGrp;

    return LE_SUCCESS;
}

void leRadioButtonGroup_Destroy(leRadioButtonGroup* grp)
{
    if(grp == NULL)
        return;

    leArray_Clear(&grp->buttonList);
    
    LE_FREE(grp);
}

leResult leRadioButtonGroup_AddButton(leRadioButtonGroup* grp,
                                      leRadioButtonWidget* btn)
{
    if(grp == NULL || grp->initialized != LE_TRUE)
        return LE_FAILURE;

    if(leArray_Find(&grp->buttonList, btn) != -1)
        return LE_FAILURE;

    if(grp->buttonList.size > 0)
    {
        btn->fn->deselect(btn);
    }
    else
    {
        grp->selected = btn;
    }
    
    leArray_PushBack(&grp->buttonList, btn);
    
    btn->group = grp;

    return LE_SUCCESS;
}

leResult leRadioButtonGroup_RemoveButton(leRadioButtonGroup* grp,
                                         leRadioButtonWidget* btn)
{
    if(grp == NULL || grp->initialized != LE_TRUE)
        return LE_FAILURE;

    if(leArray_Find(&grp->buttonList, btn) == -1)
        return LE_FAILURE;

    leArray_Remove(&grp->buttonList, btn);

    btn->group = NULL;
    
    btn->fn->setSelected(btn);
    
    if(grp->buttonList.size == 0)
    {
        leRadioButtonGroup_Destroy(grp);
    }
    
    return LE_SUCCESS;
}

leResult leRadioButtonGroup_SelectButton(leRadioButtonGroup* grp,
                                         leRadioButtonWidget* btn)
{
    if(grp == NULL || grp->initialized != LE_TRUE || grp->selected == btn)
        return LE_FAILURE;

    if(leArray_Find(&grp->buttonList, btn) == -1)
        return LE_FAILURE;

    grp->selected->fn->deselect(grp->selected);
    btn->fn->select(btn);

    grp->selected = btn;

    return LE_SUCCESS;
}

#endif // LE_RADIOBUTTON_WIDGET_ENABLED