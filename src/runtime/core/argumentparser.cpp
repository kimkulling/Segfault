#include "core/argumentparser.h"
#include "core/tokenizer.h"

namespace segfault::core {

using namespace ::cppcore;

static constexpr char option[] = "--";
static constexpr char tokenArgIn = '<';
static constexpr char tokenArgOut = '>';

ArgumentParser::Argument::Argument() :
        mArgument(""), mDesc(""), mNumArgs(0) {
    // empty
}

ArgumentParser::Argument::Argument(const std::string &arg, const std::string &desc, uint32_t numArgs) :
        mArgument(arg), mDesc(desc), mNumArgs(numArgs) {
    // empty
}

static bool parseExpectedArgs(const std::string &userDefinedArgs, const std::string &desc,
        TArray<ArgumentParser::Argument> &supportedArgs, std::string &error) {
    error.clear();
    StringArray extractedArgs, extractedDescs;
    Tokenizer::tokenize(userDefinedArgs, extractedArgs, ":");
    Tokenizer::tokenize(desc, extractedDescs, ":");
    uint32_t numParam(0);
    if (extractedArgs.size() != extractedDescs.size()) {
        error += "Number of arguments and descriptions do not match";
        return false;
    }

    for (uint32_t i = 0; i < extractedArgs.size(); ++i) {
        std::string arg = extractedArgs[i];
        std::string currentDesc = extractedDescs[i];
        if (ArgumentParser::parseArgParameter(arg, numParam)) {
            supportedArgs.add(ArgumentParser::Argument(ArgumentParser::getBlankArgument(arg),
                    currentDesc, numParam));
        }
    }

    return true;
}

ArgumentParser::ArgumentParser(int32_t argc, const char *ppArgv[], const std::string &supportedArgs, const std::string &desc) :
        mCurrentIndex(0), mIsValid(true) {
    // Parse and store the expected arguments
    const size_t optionLen = ::strlen(option);
    std::string error;
    if (!parseExpectedArgs(supportedArgs, desc, mSupportedArguments, error)) {
        //osre_error(Tag, "Error while parsing arguments: " + error);
        mIsValid = false;
        return;
    }

    const uint32_t argcU = static_cast<uint32_t>(argc);
    if (argc > 1) {
        while (mCurrentIndex < argcU) {
            std::string argument(ppArgv[mCurrentIndex]);
            const std::string::size_type pos = argument.find(option);
            if (std::string::npos != pos) {
                argument = argument.substr(pos + optionLen, argument.size() - (pos + optionLen));
                if (isSupported(argument)) {
                    if (const uint32_t numValues = getNumValues(argument)) {
                        // Ensure there are enough values and none of them is a new option
                        if (mCurrentIndex + numValues >= argcU) {
                            setInvalid();
                        } else {
                            for (uint32_t valIdx = mCurrentIndex + 1;
                                 valIdx <= mCurrentIndex + numValues;
                                 ++valIdx) {
                                std::string tmpVal(ppArgv[valIdx]);
                                const std::string::size_type pos1 = tmpVal.find(option);
                                if (pos1 != std::string::npos) {
                                    setInvalid();
                                    break;
                                }
                            }
                        }
                    }
                        
                    // Store the data if its valid
                    if (mIsValid) {
                        if ((mCurrentIndex + 1) < static_cast<uint32_t>(argc)) {
                            ++mCurrentIndex;
                            mDetectedArgs.add(argument);
                            std::string content(ppArgv[mCurrentIndex]);
                            mStoredArguments.add(content);
                        } else {
                            mDetectedArgs.add(argument);
                            mStoredArguments.add("");
                        }
                    }
                } else {
                    setInvalid();
                }
            }
            ++mCurrentIndex;
        }
    }

    // validate incoming arguments
    if (!validateArguments(argc, ppArgv)) {
        setInvalid();
    }
    mCurrentIndex = 0;
}

std::string ArgumentParser::showHelp() const {
    if (mSupportedArguments.isEmpty()) {
        return std::string("");
    }

    std::string helpMsg;
    for (uint32_t i = 0; i < mSupportedArguments.size(); i++) {
        const Argument argument = mSupportedArguments[i];
        const std::string arg = argument.mArgument;
        const std::string desc = argument.mDesc;
        const uint32_t numargs = argument.mNumArgs;

        helpMsg += "\t: ";
        helpMsg += arg;
        helpMsg += "\t: ";
        helpMsg += desc;
        helpMsg += "\n";
        if (numargs > 0) {
            helpMsg += "\n\tNumber of arguments: ";
            char buffer[512];
            memset(buffer, '\0', 512);
            sprintf(buffer, "%d\n", numargs);
            helpMsg += std::string(buffer);
        }
    }

    return helpMsg;
}

bool ArgumentParser::getNext(std::string &arg, std::string &value) {
    if (mSupportedArguments.isEmpty() || mStoredArguments.isEmpty() ||
            mCurrentIndex >= mSupportedArguments.size()) {
        return false;
    }

    arg = mSupportedArguments[mCurrentIndex].mArgument;
    value = mStoredArguments[mCurrentIndex];
    ++mCurrentIndex;

    return true;
}

void ArgumentParser::clear() {
    mSupportedArguments.clear();
    mDetectedArgs.clear();
    mStoredArguments.clear();
    mCurrentIndex = 0;
    mIsValid = true;
}

bool ArgumentParser::isSupported(const std::string &arg) const {
    bool res(false);
    for (uint32_t index = 0; index < mSupportedArguments.size(); ++index) {
        if (arg == mSupportedArguments[index].mArgument) {
            res = true;
            break;
        }
    }

    return res;
}

uint32_t ArgumentParser::getNumValues(const std::string &argument) const {
    if (argument.empty()) {
        return 0;
    }

    uint32_t numValues(0);
    for (uint32_t index = 0; index < mSupportedArguments.size(); ++index) {
        if (mSupportedArguments[index].mArgument == argument) {
            numValues = mSupportedArguments[index].mNumArgs;
            break;
        }
    }

    return numValues;
}

const std::string &ArgumentParser::getArgument(const std::string &arg) const {
    static std::string Dummy;
    if (arg.empty()) {
        return Dummy;
    }

    for (uint32_t index = 0; index < mDetectedArgs.size(); ++index) {
        if (mDetectedArgs[index] == arg) {
            return mStoredArguments[index];
        }
    }

    return Dummy;
}

bool ArgumentParser::hasArgument(const std::string &argument) const {
    bool result(false);
    for (uint32_t idx = 0; idx < mDetectedArgs.size(); ++idx) {
        if (mDetectedArgs[idx] == argument) {
            result = true;
            break;
        }
    }
    return result;
}

bool ArgumentParser::hasValidArgs() const {
    return mIsValid;
}

bool ArgumentParser::parseArgParameter(const std::string &argument, uint32_t &numPara) {
    if (argument.empty()) {
        return true;
    }

    std::string paraStr("");
    bool paraDefOpen(false);
    for (uint32_t i = 0; i < argument.size(); i++) {
        if (argument[i] == tokenArgIn) {
            if (paraDefOpen) {
                numPara = 0;
                return false;
            }
            paraDefOpen = true;
        } else if (argument[i] == tokenArgOut) {
            if (!paraDefOpen) {
                numPara = 0;
                return false;
            }
            paraDefOpen = false;

        } else if (paraDefOpen) {
            paraStr += argument[i];
        }
    }

    if (!paraDefOpen) {
        numPara = atoi(paraStr.c_str());
    } else {
        return false;
    }

    return true;
}

std::string ArgumentParser::getBlankArgument(const std::string &argument) {
    std::string blankArg("");

    // Empty, return it
    if (argument.empty()) {
        return blankArg;
    }

    // Check if any token for parameter definition is there
    if (std::string::size_type pos = argument.find(tokenArgIn); std::string::npos == pos) {
        return argument;
    }

    // Clean from parameter-specific token
    uint32_t i(0);
    while (argument[i] != tokenArgIn) {
        blankArg += argument[i];
        ++i;
    }

    return blankArg;
}

bool ArgumentParser::validateArguments(int32_t argc, const char *ppArgv[]) {
    bool valid(true);
    for (uint32_t i = 1; i < static_cast<uint32_t>(argc); ++i) {
        std::string incomingArg(ppArgv[i]);
        std::string::size_type pos = incomingArg.find("--");
        if (std::string::npos != pos) {
            bool supported(false);
            for (uint32_t j = 0; j < mSupportedArguments.size(); ++j) {
                const std::string searchFor("--" + mSupportedArguments[j].mArgument);
                if (incomingArg == searchFor) {
                    supported = true;
                    break;
                }
            }
            if (!supported) {
                mLastError = "Invalid argument: " + incomingArg;
                valid = false;
                break;
            }
        }
    }

    return valid;
}

void ArgumentParser::setInvalid() {
    mIsValid = false;
}

} // namespace segfault::core
