#include "CmTimer.h"
#include "CodalCompat.h"
#include "CodalConfig.h"
#include "CodalDmesg.h"
#include "CodalFiber.h"
#include "codal_target_hal.h"
#include <bluepill/bluepill.h>
#include <logger/logger.h>
#include <bootloader/bootloader.h>
#include <cocoos.h>
#include "tasks.h"

namespace codal
{
    namespace _cm
    {
        Timer *Timer::instance;
        static CODAL_TIMESTAMP trigger_period;
        static CODAL_TIMESTAMP last_trigger;

#ifdef NOTUSED
        //  cocoOS context and objects.
        static struct TimerContext {
        } context;
        static Sem_t timer_semaphore;
        static void timer_task(void);
#endif  //  NOTUSED

        Timer::Timer() : codal::Timer() {
            initialised = false;
            instance = this;
            prev = 0;
            trigger_period = 0;
            last_trigger = 0;
        }

        extern "C" void TIM5_IRQHandler() {
#ifdef TODO
            auto h = &Timer::instance->TimHandle;

            if (__HAL_TIM_GET_FLAG(h, TIM_FLAG_CC1) == SET)
            {
                if (__HAL_TIM_GET_IT_SOURCE(h, TIM_IT_CC1) == SET)
                {
                    __HAL_TIM_CLEAR_IT(h, TIM_IT_CC1);
                    __HAL_TIM_CLEAR_FLAG(h, TIM_FLAG_CC1);
                    ZTimer::instance->trigger();
                }
            }
#endif  //  TODO
        }

        void tick_callback() {
            //  Will be called at every millisecond tick.  Needed to keep CODAL scheduler running.
            //  Warning: This is called from an Interrupt Service Routine.  Don't trigger any interrupts or call slow functions.
            if (!Timer::instance) { return; }  //  No timer to trigger, quit.
            CODAL_TIMESTAMP now = millis();
            //  If we have exceed the tick period (4 millisec)...
            if (last_trigger + (SCHEDULER_TICK_PERIOD_US / 1000) <= now) {
                last_trigger = now;
                Timer::instance->trigger();  //  Trigger the CODAL Scheduler.
            }
        }

        void alarm_callback() {
            //  Will be called when an alarm is triggered.  Needed to keep CODAL scheduler running, which sets alarms every few seconds.
            //  Warning: This is called from an Interrupt Service Routine.  Don't trigger any interrupts or call slow functions.
            if (!Timer::instance) { return; }  //  No timer to trigger, quit.
            Timer::instance->trigger();        //  Trigger the CODAL Scheduler.
        }

        void Timer::init() {
            if (initialised) { return; }  //  If already initialised, quit.
            debug_println("timer init"); ////
            initialised = true;
            prev = millis();

#ifdef NOTUSED
            //  Create a semaphore to signal the Timer Task for a alarm or tick interrupt.
            timer_semaphore = sem_bin_create(0);  //  Binary Semaphore: Will wait until signalled.

            //  Create the cocoOS Timer Task.
            task_create(
                timer_task,   //  Task will run this function.
                &context,     //  task_get_data() will be set to the context object.
                20,           //  Priority 20
                NULL, 0, 0);
#endif  //  NOTUSED

            //  Set the callbacks that will signal the Timer Task via Semaphore.
            target_set_tick_callback(tick_callback);
            target_set_alarm_callback(alarm_callback);

            //  If we were asked to set a trigger before init(), set it now.
            if (trigger_period > 0) {
                // debug_print("init calling triggerIn... ");
                triggerIn(trigger_period);
                trigger_period = 0;
            }

            //  Start the background tasks.
            start_background_tasks();

            //  Tell bootloader to call restart_callback() when it needs to restart.
            bootloader_set_restart_callback(restart_callback);

#ifdef TODO
            TimHandle.Instance = TIM5;

            TimHandle.Init.Period = 0xFFFFFFFF;
            TimHandle.Init.Prescaler = (uint32_t)((SystemCoreClock / 1000000) - 1);
            TimHandle.Init.ClockDivision = 0;
            TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;

            if (HAL_TIM_OC_Init(&TimHandle) != HAL_OK)
                CODAL_ASSERT(0);

            NVIC_SetPriority(TIM5_IRQn, 0);
            NVIC_EnableIRQ(TIM5_IRQn);
            HAL_TIM_OC_Start(&TimHandle, TIM_CHANNEL_1);
#endif  //  TODO
        }

        void Timer::triggerIn(CODAL_TIMESTAMP t) {
            //  Set alarm for millis() + t millisecs.
            if (!initialised) {
                //  If we are called before init(), remember the trigger and set during init().  CODAL Scheduler calls the timer before it's ready.
                //  There seems to be a circular dependency, so we resolve by deferring the trigger until the scheduler has completely started up.
                trigger_period = t;
                debug_println("triggerIn b4 init!");
                return;
            }
            //  debug_print("triggerIn "); debug_println((size_t) t); debug_flush(); debug_printhex_unsigned(platform_get_alarm()); debug_print(" "); debug_flush(); ////
            platform_set_alarm(t);
            //  debug_printhex_unsigned(platform_get_alarm()); debug_print(" "); debug_flush(); ////
#ifdef TODO
            if (t < 20)
                t = 20;

            this->syncRequest(); // is this needed?

            target_disable_irq();
            __HAL_TIM_DISABLE_IT(&TimHandle, TIM_IT_CC1);
            __HAL_TIM_SET_COMPARE(&TimHandle, TIM_CHANNEL_1,
                                (uint32_t)(__HAL_TIM_GET_COUNTER(&TimHandle) + t));
            __HAL_TIM_ENABLE_IT(&TimHandle, TIM_IT_CC1);
            target_enable_irq();
#endif  //  TODO
        }

        void Timer::syncRequest() {
            if (!initialised) {
                debug_print("sync! ");  //  Warning: syncRequest called before init
                return;
            }
            target_disable_irq();
            volatile uint32_t curr = millis();
            volatile uint32_t delta = curr - this->prev;  //  In milliseconds.

            // update the hal...
            // uwTick += delta;
            this->prev = curr;
            this->sync(delta * 1000);  //  Sync expects microseconds.
            target_enable_irq();            
            //  debug_print("timer sync "); debug_println((size_t) delta * 1000);
        }

        extern "C" void wait_us(uint32_t us) {
            target_wait_us(us);
        }

#ifdef NOTUSED
        static void timer_task(void) {
            //  cocoOS task that runs forever waiting for the timer semaphore to be signalled by the alarm and tick interrupts.
            task_open();  //  Start of the task. Must be matched with task_close().  
            for (;;) {
                sem_wait(timer_semaphore);           //  Wait for the semaphore to be signalled.
                debug_print("A>> "); ////
                //  if (!Timer::instance) { continue; }  //  No timer to trigger, quit.
                //  Timer::instance->trigger();          //  Trigger the CODAL Scheduler.
            }
            task_close();  //  End of the task. Should not come here.
        }
#endif  //  NOTUSED

    }  //  namespace _cm

} // namespace codal
