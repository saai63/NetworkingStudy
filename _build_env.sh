
ANACONDA_PATH="${HOME}/anaconda3"
MINICONDA_PATH="${HOME}/miniconda3"
CONDA_PATH=""
CONDA_ENV_NAME="networkingstudy"

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
CONDA_ENV_FILE="${SCRIPT_DIR}/${CONDA_ENV_NAME}.yml"
ENV_HASH_FILE="${SCRIPT_DIR}/${CONDA_ENV_NAME}.md5sum"

if [ -d ${ANACONDA_PATH} ]; then
    echo "Anaconda is installed"
    CONDA_PATH=${ANACONDA_PATH}
elif [ -d ${MINICONDA_PATH} ]; then
    echo "Miniconda is installed"
    CONDA_PATH=${MINICONDA_PATH}
else
    echo "Neither anaconda or miniconda are installed. Exiting!!"
    return
fi

if [ ! -e ${CONDA_ENV_FILE} ]; then
    echo "${CONDA_ENV_FILE} does not exist. Exiting!!"
fi

if [ -d "${CONDA_PATH}/envs/${CONDA_ENV_NAME}" ]; then
    echo "Virtual environment is already created"
    md5sum -c "${ENV_HASH_FILE}" >/dev/null 2>&1
    if [ $? != 0 ]; then
        echo "Looks like yml has been modified"
        "${CONDA_PATH}/bin/conda" env update --prefix "${CONDA_PATH}/envs/${CONDA_ENV_NAME}" --file "${CONDA_ENV_FILE}" --prune && md5sum "${CONDA_ENV_FILE}" > "${ENV_HASH_FILE}"
    fi
else
    echo "Virtual environment doesn't exist. Creating one now"
    "${CONDA_PATH}/bin/conda" env update --prefix "${CONDA_PATH}/envs/${CONDA_ENV_NAME}" --file "${CONDA_ENV_FILE}" --prune && md5sum "${CONDA_ENV_FILE}" > "${ENV_HASH_FILE}"
fi
if [ $? == 0 ]; then
    source ${CONDA_PATH}/bin/activate ${CONDA_PATH}/envs/${CONDA_ENV_NAME}
else
    echo "Failed to create or actiavte the virtual environment. Exiting"
fi
