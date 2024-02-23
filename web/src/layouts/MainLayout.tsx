import React from 'react'
import { Outlet } from 'react-router-dom'
import FlashMessage from '../components/FlashMessage'
import Header from './Header'

export default function MainLayout() {
  return (
    <React.Fragment>
      <Header />
      <FlashMessage />
      <Outlet />
    </React.Fragment>
  )
}
