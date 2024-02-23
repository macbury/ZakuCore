import React from "react"
import { FrameHexagon } from "@arwes/core";
import styled from '@emotion/styled';
import CenteredContent from "../../components/CenteredContent"

const GPIO = styled.nav`
  width: 100%;
  @media only screen and (min-width: 992px) {
    width: 640px;
  }
  display: flex;
  position: absolute;
  flex-direction: row;
  padding: 0px 20px;

  .phys {
    color: rgba(255, 255, 255, 0.6);
    font-size: .8em;
    opacity: .8;
    position: absolute;
    left: 32px;
    text-indent: 0;
  }

  /* &:before {
    content: '';
    display: block;
    width: 58px;
    position: absolute;
    left: 50%;
    height: 100%;
    background: #073642;
    top: 0;
  } */

  .side {
    backdrop-filter: blur(8px);
    background-color: rgba(5, 198, 193, 0.4);
    flex: 1;
    position: relative;

    .connector {
      width: 25px;
      top: 0px;
      bottom: 0px;
    }

    &.left {
      .connector {
        right: 0px;
      }

      a {
        text-indent: 10px;
        border-top-right-radius: 13px;
        border-bottom-right-radius: 13px;
      }

      .phys {
        text-align: right;
        left: auto;
        right: 32px;
      }
    }

    &.right {
      .connector {
        left: 0px;
      }

      a {
        text-indent: 35px;
        border-top-left-radius: 13px;
        border-bottom-left-radius: 13px;
      }

      .pin {
        left: 4px;
      }
    }
  }

  ul {
    padding: 0px;
    margin: 0px;
    list-style: none;
    margin-top: 5px;

    li {
      padding: 0;
      margin-bottom: 5px;
      list-style: none;
      text-decoration: none;
      ::marker {
        content: "";
        display: none;
      }
    }
  }

  .connector {
    position: absolute;
    right: 0px;
    background: #073642;
    top: 0px;
    bottom: 0px;
  }

  a {
    display: block;
    position: relative;
    font-size: 14px;
    line-height: 24px;
    height: 24px;
    color: #E9E5D2;

    &:hover, &:hover .phys {
      background: #f5f3ed;
      color: #063541;
    }

    .default {
      padding-left: 10px;
      padding-right: 5px;
    }
  }

  .pin {
    display: block;
    border: 1px solid transparent;
    border-radius: 50%;
    width: 16px;
    height: 16px;
    background: #859900;
    position: absolute;
    right: 4px;
    top: 4px;
  }

  .pin:after {
    content: '';
    display: block;
    border-radius: 100%;
    background: #FDF6E3;
    position: absolute;
    left: 4px;
    top: 4px;
    width: 6px;
    height: 6px;
  }
`

export default function LightsSetupPage() {
  return (
    <CenteredContent>
      <GPIO>
        <div className="side left">
          <div className="connector"></div>
          <ul>
            <li>
              <a href="#">
                <span className="phys">1</span>
                <span className="default">
                  3v3 Power
                </span>

                <span className="pin"></span>
              </a>
            </li>

            <li>
              <a href="#">
              <span className="phys">2</span>
                <span className="default">
                  GPIO01
                </span>
                <small>Eyes</small>

                <span className="pin"></span>
              </a>
            </li>
            <li>
              <a href="#">
              <span className="phys">2</span>
                <span className="default">
                  GPIO01
                </span>
                <small>Eyes</small>

                <span className="pin"></span>
              </a>
            </li>
            <li>
              <a href="#">
              <span className="phys">2</span>
                <span className="default">
                  GPIO01
                </span>
                <small>Eyes</small>

                <span className="pin"></span>
              </a>
            </li>
            <li>
              <a href="#">
              <span className="phys">2</span>
                <span className="default">
                  GPIO01
                </span>
                <small>Eyes</small>

                <span className="pin"></span>
              </a>
            </li>
            <li>
              <a href="#">
              <span className="phys">2</span>
                <span className="default">
                  GPIO01
                </span>
                <small>Eyes</small>

                <span className="pin"></span>
              </a>
            </li>
          </ul>
        </div>

        <div className="side right">
          <div className="connector"></div>
          <ul>
            <li>
              <a href="#">
                <span className="phys">1</span>
                <span className="default">
                  3v3 Power
                </span>

                <span className="pin"></span>
              </a>
            </li>

            <li>
              <a href="#">
              <span className="phys">2</span>
                <span className="default">
                  GPIO01
                </span>
                <small>Eyes</small>

                <span className="pin"></span>
              </a>
            </li>
            <li>
              <a href="#">
              <span className="phys">2</span>
                <span className="default">
                  GPIO01
                </span>
                <small>Eyes</small>

                <span className="pin"></span>
              </a>
            </li>
            <li>
              <a href="#">
              <span className="phys">2</span>
                <span className="default">
                  GPIO01
                </span>
                <small>Eyes</small>

                <span className="pin"></span>
              </a>
            </li>
            <li>
              <a href="#">
              <span className="phys">2</span>
                <span className="default">
                  GPIO01
                </span>
                <small>Eyes</small>

                <span className="pin"></span>
              </a>
            </li>
            <li>
              <a href="#">
              <span className="phys">2</span>
                <span className="default">
                  GPIO01
                </span>
                <small>Eyes</small>

                <span className="pin"></span>
              </a>
            </li>
          </ul>
        </div>
      </GPIO>
    </CenteredContent>
  )
}
