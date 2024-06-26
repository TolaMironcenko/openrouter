const dnsenabled = document.querySelector('#dnsenable')
const setdnssettingsbutton = document.querySelector('#set_dns_settings')
const confirmdnsform = document.querySelector('.confirmdnsform')
const canceldnsbutton = document.querySelector('#canceldns')
const confirmdnsbutton = document.querySelector('#confirmdns')

const get_dns_enabled_status = () => {
    fetch(routes.dns_status(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem("token")}"}`
    }).then(res => res.json()).then(jsondata => {
        if (jsondata.enabled === '1') {
            dnsenabled.checked = true
            // dnssettingbody.classList.add('active')
            return;
        }
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

get_dns_enabled_status()

const set_dns_enabled_status = (status) => {
    fetch(routes.dns_status_set(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem("token")}","enabled":"${status?1:0}"}`
    }).then(res => res.json()).then(jsondata => {
        if (jsondata.enabled === '1') {
            dnsenabled.checked = true
            // dnssettingbody.classList.add('active')
        }
        notification("Настройки dns сохранены", "success")
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

setdnssettingsbutton.addEventListener('click', () =>{
    confirmdnsform.classList.add('active')
})
canceldnsbutton.addEventListener('click', () => {
    confirmdnsform.classList.remove('active')
})

confirmdnsbutton.addEventListener('click', () => {
    set_dns_enabled_status(dnsenabled.checked)
    confirmdnsform.classList.remove('active')
})
