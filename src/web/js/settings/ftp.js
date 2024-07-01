const ftpenabled = document.querySelector('#ftpenable')
const setftpsettingsbutton = document.querySelector('#set_ftp_settings')

const get_ftp_enabled_status = () => {
    fetch(routes.ftp_status(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem("token")}"}`
    }).then(res => res.json()).then(jsondata => {
        if (jsondata.enabled === '1') {
            ftpenabled.checked = true
            // ftpsettingbody.classList.add('active')
            return;
        }
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

const set_ftp_enabled_status = () => {
    fetch(routes.ftp_status_set(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem("token")}","enabled":"${ftpenabled.checked?1:0}"}`
    }).then(res => res.json()).then(jsondata => {
        if (jsondata.enabled === '1') {
            ftpenabled.checked = true
            // ftpsettingbody.classList.add('active')
        }
        notification("Настройки ftp сохранены", "success")
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

setftpsettingsbutton.addEventListener('click', () =>{
    get_confirm_form("Confirm ftp settings?", set_ftp_enabled_status)
})
