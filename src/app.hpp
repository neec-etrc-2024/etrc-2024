#ifdef __cplusplus
extern "C" {
#endif

#include "ev3api.h"

/* タスク優先度 */
#define MAIN_PRIORITY (TMIN_APP_TPRI + 1)      /* メインタスク */
#define SET_PARAM_PRIORITY (TMIN_APP_TPRI + 2) /* パラメータ設定タスク */
#define UPDATE_PRIORITY (TMIN_APP_TPRI + 2)    /* 情報更新タスク */
#define CALC_PRIORITY (TMIN_APP_TPRI + 2)      /* 制御計算タスク */

/* タスク周期の定義 */
#define CYC_PERIOD (10 * 1000) /* タスク:10msec周期 */

#ifndef STACK_SIZE
#define STACK_SIZE (4096)
#endif /* STACK_SIZE */

#ifndef TOPPERS_MACRO_ONLY

extern void main_task(intptr_t exinf);
extern void update_task(intptr_t exinf);    // 情報更新
extern void calc_task(intptr_t exinf);      // 制御計算
extern void set_param_task(intptr_t exinf); // パラメータ設定

#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif
