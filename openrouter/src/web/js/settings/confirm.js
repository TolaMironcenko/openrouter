const confirmform = document.querySelector('#confirmform')
const confirmformheader = document.querySelector('#confirmformheader')
const cancelbutton = document.querySelector('#cancel')
const confirmformbuttons = document.querySelector('#confirmformbuttons')

const defaultcancelreqfunc = () => {
    confirmform.classList.remove('active')
}

const get_confirm_form = (header, reqfunc = defaultcancelreqfunc, cancelreqfunc = defaultcancelreqfunc) => {
    confirmformheader.innerHTML = header
    confirmform.classList.add('active')

    var confirmbutton = document.createElement('button')
    confirmbutton.classList.add('formbutton')
    confirmbutton.id = "confirm"
    confirmbutton.innerHTML = "Confirm"
    confirmbutton.addEventListener('click', () => {
        reqfunc()
        confirmform.classList.remove('active')
    })
    confirmformbuttons.replaceChild(confirmbutton, document.querySelector('#confirm'))

    var cancelbutton = document.createElement('button')
    cancelbutton.classList.add('formbutton')
    cancelbutton.id = "cancel"
    cancelbutton.innerHTML = "Cancel"
    cancelbutton.addEventListener('click', () => {
        cancelreqfunc()
        confirmform.classList.remove('active')
    })
    confirmformbuttons.replaceChild(cancelbutton, document.querySelector('#cancel'))
}
