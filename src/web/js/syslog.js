const syslogdata = document.querySelector('.syslogdata')
const refreshbutton = document.querySelector('#refreshbutton')

const get_syslog_data = () => {
    console.log("get_syslog_data")
    fetch(routes.syslog(), {
        method: 'GET',
        // body: `{"token":"${localStorage.getItem("token")}"}`
    }).then(data => data.text()).then(textdata => {
        syslogdata.innerHTML = textdata.replace(/\n/g,'<br>')
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

refreshbutton.addEventListener('click', get_syslog_data)
