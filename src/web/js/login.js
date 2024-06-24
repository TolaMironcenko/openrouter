const loginform = document.querySelector('.loginform')
const loginformbody = document.querySelector('.loginformbody')
const loginusername = document.querySelector("#loginusernameinput")
const loginpassword = document.querySelector("#loginpasswordinput")
const exitbutton = document.querySelector('.exitbutton')

console.log(localStorage.getItem("token"))

if (localStorage.getItem("token") === null) {
    loginform.classList.add('active')
    // loginform.style.background = "initial"
}

loginformbody.addEventListener('submit', (e) => {
    e.preventDefault()
    login(loginusername.value, loginpassword.value)
})

const login = async (username, password) => {
    const response = await fetch(routes.login(), {
        method: 'POST',
        headers: {
            Accept: 'application/json',
            // 'Access-Control-Allow-Origin': '*',
            'Content-Type': 'application/json',
        },
        // mode: 'no-cors',
        body: `{"username":"${username}","password":"${password}"}`
    }).then(data => data.json()).then(jsondata => jsondata).catch((error) => {
        if (error.message === "Unexpected end of JSON input") {
            console.log("error")
            // notification(`Ошибка на сервере: Создайте файл users.json и запишите в него первого пользователя`, "error")
        } else {
            console.log("error")
            // notification(`Ошибка на сервере: ${error}`, "error")
        }
    })
    if (response.status === "403") {
        console.log("Неправильный логин или пароль")
        // notification("Неправильный логин или пароль", "error")
    }
    if (response.token) {
        // notification("Успешно", "success")
        loginform.classList.remove("active")
        localStorage.setItem("token", response.token)
        await get_user_data()
        loginforminputusername.value = ""
        loginforminputpassword.value = ""
    }
}

exitbutton.addEventListener('click', () => {
    localStorage.clear()
    loginform.classList.add('active')
    loginusername.value = ""
    loginpassword = ""
})
