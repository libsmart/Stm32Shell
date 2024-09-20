/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32SHELL_READLINE_ABSTRACTMICRORLSTREAMSESSION_HPP
#define LIBSMART_STM32SHELL_READLINE_ABSTRACTMICRORLSTREAMSESSION_HPP

#include <libsmart_config.hpp>
#include <microrl.h>
#include <StreamSession/StreamSessionInterface.hpp>
#include "Loggable.hpp"
#include "StreamRxTx.hpp"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef __cplusplus
}
#endif /* __cplusplus */

namespace Stm32Shell::Readline {
    class Server;

    /**
     * @brief Abstract class for microrl stream session management.
     *
     * This class provides an interface and basic implementation for managing
     * microrl stream sessions. It inherits from several interfaces to provide
     * a comprehensive set of functionalities for handling stream sessions.
     *
     * Must at least implement the function executeCallback(), which is called every time
     * the user presses enter.
     */
    class AbstractMicrorlStreamSession : protected microrl_t,
                                         public Stm32Common::StreamSession::StreamSessionInterface,
                                         public Stm32Common::StreamRxTx<
                                             LIBSMART_STM32SHELL_MICRORLSTREAMSESSION_BUFFER_SIZE_RX,
                                             LIBSMART_STM32SHELL_MICRORLSTREAMSESSION_BUFFER_SIZE_TX> {
    public:
        friend Server;

        AbstractMicrorlStreamSession() : microrl_t() { ; }

        ~AbstractMicrorlStreamSession() override;

        void flush() override;

        void setup() override;

        void loop() override;

        void end() override;


        /**
         * @brief Processes input data for the microrl stream session.
         *
         * This method processes the input data received for the microrl stream session,
         * and returns the processing status.
         *
         * @param data_ptr Pointer to the input data to be processed.
         * @param len      Length of the input data.
         * @return The status of the input processing. Returns microrlOK on success,
         *         otherwise returns an error code.
         */
        virtual microrlr_t processingInput(const void *data_ptr, size_t len);

        /**
         * @brief Processes the input command string.
         *
         * This method takes a C-style string as input and processes it
         * according to the requirements of the microrl stream session.
         *
         * @param c_string The input command string to be processed.
         * @return A result code of type microrlr_t, indicating the status of the processing.
         */
        virtual microrlr_t processingInput(const char *c_string);

        /**
         * @brief Sets the echo mode for the microrl stream session.
         *
         * This function configures the echo behavior of the microrl stream
         * session. It logs informational and error messages based on the
         * outcome of the operation.
         *
         * @param echo The desired echo mode to be set.
         * @return microrlr_t The status of the echo setting operation.
         */
        virtual microrlr_t setEcho(microrl_echo_t echo);

        /**
         * @brief Sets the prompt string for the microrl stream session.
         *
         * This method sets the prompt string that will be displayed to the user in the microrl stream session.
         * The prompt string is updated using the microrl library function microrl_set_prompt.
         *
         * @param prompt_str The new prompt string to be set.
         * @return microrlr_t Returns microrlOK upon success. Otherwise, returns an error code defined in the microrlr_t enum.
         */
        virtual microrlr_t setPrompt(const char *prompt_str);

        /**
         * @brief Retrieves the pre-command callback function for the microrl stream session.
         *
         * This method returns a function pointer to the pre-command callback which
         * is executed before any command is processed by the microrl interface.
         *
         * @return Function pointer to the pre-command callback of type microrl_pre_cmd_fn.
         */
        static microrl_pre_cmd_fn getPreCommandCallback();

        /**
         * @brief Retrieves the post command callback function for the microrl stream session.
         *
         * This method returns a callback function that is triggered after executing a command
         * in the microrl stream session.
         *
         * @return A microrl_post_cmd_fn type function pointer that is used as the post command callback.
         */
        static microrl_post_cmd_fn getPostCommandCallback();

    protected:
        /**
         * @brief Initializes the microrl library with provided output and execute callbacks.
         *
         * This method sets up the microrl instance by assigning the provided output
         * and execute function callbacks. It also logs the initialization process
         * and checks for any initialization errors.
         *
         * @param out_fn The output function callback used by microrl for output operations.
         * @param exec_fn The execute function callback used by microrl to process commands.
         * @return Returns an enum value of type microrlr_t indicating the status of the initialization.
         *         Possible return values include microrlOK, microrlERR, microrlERRPAR, microrlERRTKNNUM,
         *         microrlERRCLFULL, or microrlERRCPLT.
         */
        virtual microrlr_t microrlInit(microrl_output_fn out_fn, microrl_exec_fn exec_fn);

        /**
         * @brief Sets the execute callback function for microrl stream sessions.
         *
         * This function assigns the provided callback function, which will be called
         * whenever the user presses enter in the microrl session.
         *
         * @param exec_fn The callback function to be set for execution upon user input.
         * @return The status code of the operation. Returns microrlOK if the callback
         *         was successfully set, otherwise returns an appropriate error code.
         */
        virtual microrlr_t setExecuteCallback(microrl_exec_fn exec_fn);

        /**
         * @brief Sets the autocompletion callback for the microrl stream session.
         *
         * This method assigns a function to be called for autocompletion purposes
         * when the user presses the tab key. It sets the callback function
         * that will provide completion suggestions.
         *
         * @param get_completion_fn The function pointer for the autocompletion callback.
         *                          This function should return completion suggestions
         *                          based on the current input.
         * @return microrlr_t Returns an enumerated type indicating the status of setting
         *                    the callback. It can be microrlOK if the operation is successful
         *                    or an appropriate error code if it fails.
         */
        virtual microrlr_t setCompleteCallback(microrl_get_compl_fn get_completion_fn);

        /**
         * @brief Sets the SIGINT (Ctrl+C) callback function for the microrl session.
         *
         * This method allows the user to specify a callback function that will be invoked
         * when a SIGINT (Ctrl+C) signal is received during a microrl session.
         *
         * @param sigint_fn The callback function to be set for handling SIGINT.
         * @return Returns a `microrlr_t` enumeration value indicating the status of the operation.
         *         - microrlOK: The callback was set successfully.
         *         - microrlERR: A general error occurred.
         *         - microrlERRPAR: A parameter error occurred.
         *         - microrlERRTKNNUM: Too many tokens error.
         *         - microrlERRCLFULL: Command line is full.
         *         - microrlERRCPLT: Auto-completion error.
         */
        virtual microrlr_t setSigintCallback(microrl_sigint_fn sigint_fn);

        /**
         * @brief Retrieve the current version of the microrl library.
         *
         * This method logs an informational message using the Stm32ItmLogger and then
         * calls the microrl_get_version function to obtain the version information.
         *
         * @return The version of the microrl library as a 32-bit unsigned integer.
         */
        virtual uint32_t getVersion();

        /**
         * @brief Execute the callback function associated with this session.
         *
         * This is a pure virtual function that must be implemented by the derived class.
         * The implementation should handle the necessary operations when the callback is triggered.
         *
         * @return An integer value indicating the result of the callback execution.
         */
        virtual int executeCallback(int argc, const char *const *argv) = 0;

    private:
        /**
         * @brief Output function for microrl library
         *
         * This method is responsible for handling the output from the microrl instance.
         * It writes the provided string to the appropriate output medium.
         *
         * @param str The string to be output
         *
         * @return Returns 0 upon successful completion
         *
         * @note This function logs the output action for debugging purposes at the DEBUGGING severity level.
         */
        int microrlOutputCb(const char *str);

        /**
         * @brief Execute a command within the microrl context
         *
         * This method is responsible for executing a given command within the microrl instance.
         * It processes the arguments provided and logs information about the command execution.
         *
         * @param argc The number of arguments passed to the command
         * @param argv An array of argument strings
         *
         * @return Returns 0 upon successful execution of the command, 1 otherwise
         */
        int microrlExecCb(int argc, const char *const *argv);

        /**
         * @brief Provide command completion suggestions.
         *
         * This method returns an array of strings representing possible completions
         * for the current command input. It is used within the microrl library to
         * assist users with command completion.
         *
         * @param argc The number of arguments currently input
         * @param argv An array of argument strings provided so far
         *
         * @return Returns a `char**` array of possible command completions or nullptr if no completions are found
         */
        char **microrlCompleteCb(int argc, const char *const *argv);

        /**
         * @brief Handle the SIGINT signal for the given microrl instance.
         *
         * This function is intended to handle the interrupt signal (SIGINT) for
         * the specified microrl instance.
         */
        void microrlSigintCb();

        /**
         * @brief Callback function executed before a microrl command is processed.
         *
         * This method is called prior to executing a microrl command, allowing
         * for any necessary preprocessing.
         *
         * @param argc Number of arguments passed to the command.
         * @param argv Array of arguments passed to the command.
         */
        void microrlPreCommandCb(int argc, const char *const *argv);

        /**
         * @brief Callback function to handle post-command execution in microrl.
         *
         * This function is called after a command has been executed in the microrl
         * environment. It provides an opportunity to capture the results and arguments
         * passed to the executed command.
         *
         * @param res The result of the command execution.
         * @param argc The number of arguments passed to the command.
         * @param argv The array of arguments passed to the command.
         */
        void microrlPostCommandCb(int res, int argc, const char *const *argv);


        /**
         * @brief Variable to store the state of the IAC (Interpret As Command) byte in Telnet protocol.
         *
         * This variable holds the value of the IAC byte, which is used to signal that the following bytes
         * represent a command rather than data. It is an 8-bit unsigned integer initialized to zero.
         */
        uint8_t iac{};
        uint8_t iacCmd{};

    protected:
        void onWriteTx() override;

        template<class T, class Method, Method m, class... Params>
        /**
         * @brief Bounce Function
         *
         * This template function calls a member function (Method) on an object (T) passed through a microrl pointer.
         * It verifies that the given microrl pointer and the session cast from it are valid before invoking the member function.
         *
         * @tparam T The class type of the object on which the member function will be called
         * @tparam Method The type of the member function
         * @tparam m Pointer to the member function
         * @tparam Params The types of additional parameters to forward to the member function call
         *
         * @param mrl A pointer to a microrl object representing the session
         * @param params The additional parameters to be forwarded to the member function
         *
         * @return The return value of the member function call
         *
         * @note The caller is responsible for ensuring that the microrl pointer represents a valid session and the member function
         *       pointed to by m is of the correct signature and can be called with the provided parameters.
         */
        static auto bounce(microrl *mrl, Params... params) ->
            decltype(((*static_cast<T *>(mrl)).*m)(params...)) {
            assert_param(mrl != nullptr);
            T *session = static_cast<T *>(mrl);
            assert_param(session != nullptr);
            return ((*static_cast<T *>(session)).*m)(params...);
        }
    };
}

#endif
